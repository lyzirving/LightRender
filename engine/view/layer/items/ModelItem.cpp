#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <limits>
#include <cassert>

#include "ModelItem.h"
#include "ViewTransform.h"

#include "GfxMesh.h"
#include "GfxHelper.h"
#include "GfxDef.h"
#include "ShaderMgr.h"
#include "Shader.h"

#include "AssetsMgr.h"

#include "Logger.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ModelItem"

ModelItem::ModelItem(const char* name, bool fCenter, bool fScale) : LayerItem(fCenter, fScale), 
                                                                    m_name(name), m_srcRoot(), m_meshInd(0), m_mesh()
{
	if (m_name.empty())
	{
		LOG_ERR("model name is empty");
		assert(0);
	}
	loadModel();
}

ModelItem::~ModelItem() = default;

void ModelItem::draw(const std::shared_ptr<ViewTransform>& trans)
{
	const glm::mat4& viewMat = trans->getViewMat();
	const glm::mat4& prjMat = trans->getProjectMat();

	const std::shared_ptr<Shader>& objShader = ShaderMgr::get()->getShader(SHADER_OBJ);
	if (!objShader)
	{
		LOG_ERR("obj shader is null");
		assert(0);
	}
	objShader->use(true);
	objShader->setMat4(U_MAT_VIEW, viewMat);
	objShader->setMat4(U_MAT_PRJ, prjMat);
	objShader->setMat4(U_MAT_MODEL, m_modelMat * m_adjMat);

	for (auto& item : m_mesh)
	{
		item->draw(objShader);
	}

	objShader->use(false);
}

void ModelItem::loadModel()
{
	std::string path = AssetsMgr::getObj(m_name);
	int pos0 = path.find_last_of('/');
	m_srcRoot = path.substr(0, pos0);

	// use assimp load model and get aiScene
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		LOG_ERR("fail to load model from [%s], reason[%s]", path.c_str(), importer.GetErrorString());
		return;
	}
	LOG_INFO("start to load model from[%s]", path.c_str());
	processNode(scene->mRootNode, scene);
	computeAdjMat();
	LOG_INFO("finish loading model[%s]", m_name.c_str());
}

std::vector<Texture> ModelItem::loadTexture(aiMaterial* mt, aiTextureType type, uint8_t texType)
{
	std::vector<Texture> result;
	unsigned int count = mt->GetTextureCount(type);
	for (unsigned int i = 0; i < count; i++)
	{
		aiString texName;
		mt->GetTexture(type, i, &texName);
		std::string texPath = m_srcRoot + '/' + texName.C_Str();
		Texture texture{};
		texture.m_type = TexType(texType);
		texture.m_path = texPath;
		GfxHelper::get()->loadTex(texPath, texture.m_texId, texture.m_width, texture.m_height, texture.m_channel);
		result.push_back(std::move(texture));
	}
	return result;
}

void ModelItem::processNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		if (mesh != nullptr)
			m_mesh.push_back(std::move(processMesh(mesh, scene)));
	}

	// process the children for current node
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

std::shared_ptr<GfxMesh> ModelItem::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::string meshName = m_name + "@Mesh@" + std::to_string(m_meshInd++);
	std::shared_ptr<GfxMesh> result = std::make_shared<GfxMesh>(meshName.c_str());

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.m_pos.x = mesh->mVertices[i].x;
		vertex.m_pos.y = mesh->mVertices[i].y;
		vertex.m_pos.z = mesh->mVertices[i].z;
		updateMinMax(vertex.m_pos);

		vertex.m_normal.x = mesh->mNormals[i].x;
		vertex.m_normal.y = mesh->mNormals[i].y;
		vertex.m_normal.z = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0])
		{
			vertex.m_tex.x = mesh->mTextureCoords[0][i].x;
			vertex.m_tex.y = mesh->mTextureCoords[0][i].y;
		}
		result->pushBackVert(std::move(vertex));
	}

	// process the indices
	// every face represent a primitive
	// we use flag aiProcess_Triangulate to import the model, so a single primitive is a triangle
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; ++j)
			result->pushBackIndices(face.mIndices[j]);
	}

	// every mesh only has one Material
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* mt = scene->mMaterials[mesh->mMaterialIndex];
		if (mt)
		{
			std::vector<Texture> diffVec = loadTexture(mt, aiTextureType_DIFFUSE, TexType::DIFFUSE);
			std::vector<Texture> specVec = loadTexture(mt, aiTextureType_SPECULAR, TexType::SPECULAR);
			if (!diffVec.empty()) result->pushBackTexVec(diffVec);
			if(!specVec.empty()) result->pushBackTexVec(specVec);

			const std::shared_ptr<Material>& gfxMt = result->getMaterial();

			aiReturn ret{ aiReturn::aiReturn_SUCCESS };
			aiColor3D color;
			ret = mt->Get(AI_MATKEY_COLOR_DIFFUSE, color);
			if (ret == aiReturn::aiReturn_SUCCESS)
				gfxMt->m_kd = glm::vec3(color.r, color.g, color.b);

			ret = mt->Get(AI_MATKEY_COLOR_SPECULAR, color);
			if (ret == aiReturn::aiReturn_SUCCESS)
				gfxMt->m_ks = glm::vec3(color.r, color.g, color.b);

			ret = mt->Get(AI_MATKEY_COLOR_AMBIENT, color);
			if (ret == aiReturn::aiReturn_SUCCESS)
				gfxMt->m_ka = glm::vec3(color.r, color.g, color.b);

			ai_real val;
			ret = mt->Get(AI_MATKEY_SHININESS, val);
			if (ret == aiReturn::aiReturn_SUCCESS)
				gfxMt->m_shine = val;
		}
	}

	result->bind(false);

	LOG_INFO("finish parse mesh, info: %s", result->getMeshInfo().c_str());
	return result;
}