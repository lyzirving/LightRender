#ifndef BVH_BUILDER_H
#define BVH_BUILDER_H

#include <vector>
#include <string>
#include <memory>
#include <glm/glm.hpp>

class aiScene;
class aiNode;
class aiMesh;

struct RrtBVHNode;
struct RrtTriangle;

class BVHBuilder
{
public:
    BVHBuilder(const char* name, bool adj = false);
    virtual ~BVHBuilder();

    void build(std::vector<RrtBVHNode>& outNodes);
    void getTriangles(std::vector<RrtTriangle>& out);

protected:
    void adjust();
    void loadModel();
    void processNode(aiNode* node, const aiScene* scene);
    void processMesh(aiMesh* mesh, const aiScene* scene);

    std::string m_name;
    bool m_adjFlag;
    std::vector<RrtTriangle> m_triangles;
    glm::vec3 m_maxPos, m_minPos;

private:
    static int buildBVHWithSAH(std::vector<RrtTriangle>& triangles, std::vector<RrtBVHNode>& outNodes, int l, int r, int limit);
    static bool cmpX(const RrtTriangle& lhs, const RrtTriangle& rhs);
    static bool cmpY(const RrtTriangle& lhs, const RrtTriangle& rhs);
    static bool cmpZ(const RrtTriangle& lhs, const RrtTriangle& rhs);
    static void lowestCostSAH(std::vector<RrtTriangle>& triangles, int l, int r, float& lowCost, int& lowAxis, int& lowSplit);
};

#endif //BVH_BUILDER_H
