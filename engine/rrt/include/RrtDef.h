#ifndef RRT_DEF_H
#define RRT_DEF_H

#include <glm/glm.hpp>

static const char *RRT_SLOT_TRI  = "u_triangles";
static const char *RRT_SLOT_BVH  = "u_BVHNodes";
static const char *U_TRI_CNT     = "u_triCnt";
static const char *U_BVH_CNT     = "u_BVHNodeCnt";
static const char* U_INV_VIEW_MT = "u_invViewMat";
static const char* U_FOCAL_LEN   = "u_focalLen";

struct RrtCanvasVert
{
	glm::vec3 m_pos;

	RrtCanvasVert() : m_pos(0.f) {}
};

/*
* EncodeTriangle is used to transform data from cpu to gpu.
* make sure every field should be vec3.
*/
struct EncodeTriangle
{
    // make sure p0, p1, p2 is in counter-clockwise order
    glm::vec3 p0, p1, p2;
    glm::vec3 n0, n1, n2;
    glm::vec3 color;

    EncodeTriangle() : p0(0.f), p1(0.f), p2(0.f), n0(0.f), n1(0.f), n2(0.f), color(0.f) {}
};

/*
* EncodeBVH is used to transform data from cpu to gpu.
* make sure every field should be vec3.
*/
struct EncodeBVH
{
    // childInfo.x stores left child's index.
    // childInfo.y stores right child's index.
    // childInfo.z is reserved
    glm::vec3 childInfo;

    // posInfo.x stores children's count.
    // posInfo.y stores current node's children's start index.
    // posInfo.z is reserved
    glm::vec3 posInfo;
    glm::vec3 AA, BB;

    EncodeBVH() : childInfo(0.f), posInfo(0.f), AA(0.f), BB(0.f) {}
};

#endif // !RRT_DEF_H
