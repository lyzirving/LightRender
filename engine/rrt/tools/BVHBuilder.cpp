#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/gtc/matrix_transform.hpp>

#include <limits>
#include <algorithm>

#include "BVHBuilder.h"
#include "RrtDef.h"

#include "AssetsMgr.h"
#include "SystemUtil.h"

#include "Logger.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "BVHBuilder"

#ifdef INF
#undef INF
#endif
#define INF 114514.0

BVHBuilder::BVHBuilder(const char* name, bool adj) : m_name(name), m_adjFlag(adj),
                                                     m_triangles(), m_maxPos(-INF), m_minPos(INF)
{
    loadModel();
}

BVHBuilder::~BVHBuilder()
{
    std::vector<EncodeTriangle>().swap(m_triangles);
}

void BVHBuilder::adjust()
{
    glm::vec3 center = (m_maxPos + m_minPos) * 0.5f;
    glm::vec3 interval = m_maxPos - m_minPos;
    float scale = 2.f / std::max(std::max(interval.x, interval.y), interval.z);

    if (m_adjFlag)
    {
        for (EncodeTriangle& item : m_triangles)
        {
            item.p0 -= center;
            item.p0 *= scale;

            item.p1 -= center;
            item.p1 *= scale;

            item.p2 -= center;
            item.p2 *= scale;
        }
    }
}

void BVHBuilder::build(std::vector<EncodeBVH>& outNodes)
{
    if (m_triangles.empty())
    {
        LOG_ERR("triangle is empty");
        assert(0);
    }
    if (!outNodes.empty())
    {
        LOG_INFO("out nodes are not empty[%lu], return", outNodes.size());
        return;
    }
    LOG_INFO("start build BVH[%s]", m_name.c_str());
    int64_t startTime = SystemUtil::curTimeMs();
    buildBVHWithSAH(m_triangles, outNodes, 0, m_triangles.size() - 1, 8);
    LOG_INFO("finish build BVH, cost[%.3f]s, node size[%lu]", (SystemUtil::curTimeMs() - startTime) / 1000.f, outNodes.size());
}

int BVHBuilder::buildBVHWithSAH(std::vector<EncodeTriangle>& triangles, std::vector<EncodeBVH>& outNodes, int l, int r, int limit)
{
    if (l > r)
    {
        LOG_INFO("left[%d] is larger than right[%d]", l, r);
        return -1;
    }

    // insert a default node
    outNodes.emplace_back();
    int id = outNodes.size() - 1;
    outNodes[id].childInfo.x = outNodes[id].childInfo.y = outNodes[id].childInfo.z = 0;
    outNodes[id].posInfo.x = outNodes[id].posInfo.y = outNodes[id].posInfo.z = 0;
    outNodes[id].AA = glm::vec3(INF);
    outNodes[id].BB = glm::vec3(-INF);

    // find AABB
    for (int i = l; i <= r; ++i) 
    {
        EncodeTriangle& item = triangles[i];
        glm::vec3 min = glm::min(item.p0, glm::min(item.p1, item.p2));
        glm::vec3 max = glm::max(item.p0, glm::max(item.p1, item.p2));
        outNodes[id].AA = glm::min(outNodes[id].AA, min);
        outNodes[id].BB = glm::max(outNodes[id].BB, max);
    }

    // make a leaf node
    if ((r - l + 1) <= limit)
    {
        // children count
        outNodes[id].posInfo.x = r - l + 1;
        // start index
        outNodes[id].posInfo.y = l;
        return id;
    }

    float cost = INF;
    int axis = 0;
    int split = (l + r) / 2;

    lowestCostSAH(triangles, l, r, cost, axis, split);

    // sort by the axis with the lowest cost
    if (axis == 0) std::sort(&triangles[0] + l, &triangles[0] + r + 1, cmpX);
    if (axis == 1) std::sort(&triangles[0] + l, &triangles[0] + r + 1, cmpY);
    if (axis == 2) std::sort(&triangles[0] + l, &triangles[0] + r + 1, cmpZ);

    outNodes[id].childInfo.x = buildBVHWithSAH(triangles, outNodes, l, split, limit);
    outNodes[id].childInfo.y = buildBVHWithSAH(triangles, outNodes, split + 1, r, limit);

    return id;
}

bool BVHBuilder::cmpX(const EncodeTriangle& lhs, const EncodeTriangle& rhs)
{
    glm::vec3 lhsCenter = (lhs.p0 + lhs.p1 + lhs.p2) / 3.f;
    glm::vec3 rhsCenter = (rhs.p0 + rhs.p1 + rhs.p2) / 3.f;
    return lhsCenter.x < rhsCenter.x;
}

bool BVHBuilder::cmpY(const EncodeTriangle& lhs, const EncodeTriangle& rhs)
{
    glm::vec3 lhsCenter = (lhs.p0 + lhs.p1 + lhs.p2) / 3.f;
    glm::vec3 rhsCenter = (rhs.p0 + rhs.p1 + rhs.p2) / 3.f;
    return lhsCenter.y < rhsCenter.y;
}

bool BVHBuilder::cmpZ(const EncodeTriangle& lhs, const EncodeTriangle& rhs)
{
    glm::vec3 lhsCenter = (lhs.p0 + lhs.p1 + lhs.p2) / 3.f;
    glm::vec3 rhsCenter = (rhs.p0 + rhs.p1 + rhs.p2) / 3.f;
    return lhsCenter.z < rhsCenter.z;
}

void BVHBuilder::getTriangles(std::vector<EncodeTriangle>& out)
{
    out.assign(m_triangles.begin(), m_triangles.end());
}

void BVHBuilder::lowestCostSAH(std::vector<EncodeTriangle>& triangles, int l, int r, float& lowCost, int& lowAxis, int& lowSplit)
{
    for (int axisInd = 0; axisInd < 3; ++axisInd)
    {
        // sort the triangles by certain axis
        if (axisInd == 0)  std::sort(&triangles[0] + l, &triangles[0] + r + 1, cmpX);
        if (axisInd == 1)  std::sort(&triangles[0] + l, &triangles[0] + r + 1, cmpY);
        if (axisInd == 2)  std::sort(&triangles[0] + l, &triangles[0] + r + 1, cmpZ);

        std::vector<glm::vec3> leftMin(r - l + 1, glm::vec3(INF));
        std::vector<glm::vec3> leftMax(r - l + 1, glm::vec3(-INF));
        for (int i = l; i <= r; ++i)
        {
            EncodeTriangle& tri = triangles[i];
            int bias = ((i == l) ? 0 : 1);

            leftMin[i - l] = glm::min(leftMin[i - l - bias], glm::min(tri.p0, glm::min(tri.p1, tri.p2)));
            leftMax[i - l] = glm::max(leftMax[i - l - bias], glm::max(tri.p0, glm::max(tri.p1, tri.p2)));
        }

        std::vector<glm::vec3> rightMin(r - l + 1, glm::vec3(INF));
        std::vector<glm::vec3> rightMax(r - l + 1, glm::vec3(-INF));
        for (int i = r; i >= l; --i)
        {
            EncodeTriangle& tri = triangles[i];
            int bias = ((i == r) ? 0 : 1);

            rightMin[i - l] = glm::min(rightMin[i - l + bias], glm::min(tri.p0, glm::min(tri.p1, tri.p2)));
            rightMax[i - l] = glm::max(rightMax[i - l + bias], glm::max(tri.p0, glm::max(tri.p1, tri.p2)));
        }

        int split = l;
        float cost = INF;
        // split from l to r - 1 to calculate cost and split position
        for (int i = l; i <= r - 1; ++i)
        {
            // left side[l, i]
            glm::vec3 leftAA = leftMin[i - l];
            glm::vec3 leftBB = leftMax[i - l];
            glm::vec3 diff = leftBB - leftAA;
            float leftArea = 2 * (diff.x * diff.y + diff.x * diff.z + diff.y * diff.z);
            float leftCost = leftArea * float(i - l + 1);

            // right side[i + 1, r]
            glm::vec3 rightAA = rightMin[i - l + 1];
            glm::vec3 rightBB = rightMax[i - l + 1];
            diff = rightBB - rightAA;
            float rightArea = 2 * (diff.x * diff.y + diff.x * diff.z + diff.y * diff.z);
            float rightCost = rightArea * float(r - i);

            float totalCost = leftCost + rightCost;
            if (totalCost < cost)
            {
                cost = totalCost;
                split = i;
            }
        }

        if (cost < lowCost)
        {
            lowCost = cost;
            lowSplit = split;
            lowAxis = axisInd;
        }
    }
}

void BVHBuilder::loadModel()
{
    if (m_name.empty())
    {
        LOG_ERR("src name is empty");
        return;
    }
    std::string path = AssetsMgr::getObj(m_name);
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        LOG_ERR("fail to load model from [%s], reason[%s]", path.c_str(), importer.GetErrorString());
        return;
    }
    LOG_INFO("start to load model from[%s]", path.c_str());
    processNode(scene->mRootNode, scene);
    adjust();
    LOG_INFO("finish loading model[%s], rrt triangle size[%lu]", m_name.c_str(), m_triangles.size());
}

void BVHBuilder::processNode(aiNode* node, const aiScene* scene)
{
    for (int i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene);
    }

    for (int i = 0; i < node->mNumChildren; ++i)
    {
        processNode(node->mChildren[i], scene);
    }
}

void BVHBuilder::processMesh(aiMesh* mesh, const aiScene* scene)
{
    if (!mesh || !scene)
        return;

    if (mesh->mNumVertices == 0)
        return;

    aiVector3D aiVert, aiNormal;
    EncodeTriangle tri;

    bool normalExist = (mesh->mNormals != nullptr);

    for (int i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        if (face.mNumIndices != 3)
        {
            LOG_ERR("obj[%s]'s face[%u] index count is not 3", m_name.c_str(), i);
            continue;
        }

        aiVert = mesh->mVertices[face.mIndices[0]];
        tri.color = glm::vec3(1.f, 0.f, 0.f);
        tri.p0.x = aiVert.x;
        tri.p0.y = aiVert.y;
        tri.p0.z = aiVert.z;
        m_maxPos = glm::max(m_maxPos, tri.p0);
        m_minPos = glm::min(m_minPos, tri.p0);
        if (normalExist)
        {
            aiNormal = mesh->mNormals[face.mIndices[0]];
            tri.n0.x = aiNormal.x;
            tri.n0.y = aiNormal.y;
            tri.n0.z = aiNormal.z;
        }

        aiVert = mesh->mVertices[face.mIndices[1]];
        tri.p1.x = aiVert.x;
        tri.p1.y = aiVert.y;
        tri.p1.z = aiVert.z;
        m_maxPos = glm::max(m_maxPos, tri.p1);
        m_minPos = glm::min(m_minPos, tri.p1);
        if (normalExist)
        {
            aiNormal = mesh->mNormals[face.mIndices[1]];
            tri.n1.x = aiNormal.x;
            tri.n1.y = aiNormal.y;
            tri.n1.z = aiNormal.z;
        }

        aiVert = mesh->mVertices[face.mIndices[2]];
        tri.p2.x = aiVert.x;
        tri.p2.y = aiVert.y;
        tri.p2.z = aiVert.z;
        m_maxPos = glm::max(m_maxPos, tri.p2);
        m_minPos = glm::min(m_minPos, tri.p2);
        if (normalExist)
        {
            aiNormal = mesh->mNormals[face.mIndices[2]];
            tri.n2.x = aiNormal.x;
            tri.n2.y = aiNormal.y;
            tri.n2.z = aiNormal.z;
        }

        m_triangles.push_back(tri);
    }
}