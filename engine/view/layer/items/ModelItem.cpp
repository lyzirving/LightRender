#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>

#include <cassert>

#include "ModelItem.h"
#include "ViewTransform.h"
#include "AssetsMgr.h"

#include "Logger.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ModelItem"

ModelItem::ModelItem(const char* name) : LayerItem(), m_name(name)
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
}

void ModelItem::loadModel()
{
	std::string path = AssetsMgr::getObj(m_name);

	// use assimp load model and get aiScene
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		LOG_ERR("fail to load model from [%s], reason[%s]", path.c_str(), importer.GetErrorString());
		return;
	}
	LOG_INFO("start to load model from[%s]", path.c_str());
	LOG_INFO("finish loading model[%s]", m_name.c_str());
}