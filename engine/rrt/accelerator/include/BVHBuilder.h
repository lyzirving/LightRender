#ifndef BVH_BUILDER_H
#define BVH_BUILDER_H

#include <vector>
#include <string>
#include <memory>
#include <glm/glm.hpp>

class aiScene;
class aiNode;
class aiMesh;

struct EncodeBVH;
struct EncodeTriangle;

class BVHBuilder
{
public:
    BVHBuilder(const char* name, bool adj = false);
    virtual ~BVHBuilder();

    void build(std::vector<EncodeBVH>& outNodes);
    void getTriangles(std::vector<EncodeTriangle>& out);

protected:
    void adjust();
    void loadModel();
    void processNode(aiNode* node, const aiScene* scene);
    void processMesh(aiMesh* mesh, const aiScene* scene);

    std::string m_name;
    bool m_adjFlag;
    std::vector<EncodeTriangle> m_triangles;
    glm::vec3 m_maxPos, m_minPos;

private:
    static int buildBVHWithSAH(std::vector<EncodeTriangle>& triangles, std::vector<EncodeBVH>& outNodes, int l, int r, int limit);
    static bool cmpX(const EncodeTriangle& lhs, const EncodeTriangle& rhs);
    static bool cmpY(const EncodeTriangle& lhs, const EncodeTriangle& rhs);
    static bool cmpZ(const EncodeTriangle& lhs, const EncodeTriangle& rhs);
    static void lowestCostSAH(std::vector<EncodeTriangle>& triangles, int l, int r, float& lowCost, int& lowAxis, int& lowSplit);
};

#endif //BVH_BUILDER_H
