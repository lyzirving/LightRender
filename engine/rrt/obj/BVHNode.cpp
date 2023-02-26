#include "BVHNode.h"
#include "AABB.h"

BVHNode::BVHNode() : Hittable(), m_box(new AABB), m_left(nullptr), m_right(nullptr)
{
}

BVHNode::~BVHNode()
{
    m_box.reset();
    m_left.reset();
    m_right.reset();
}

bool BVHNode::boundingBox(AABB& box) const
{
    box = *m_box;
    return true;
}

glm::vec3 BVHNode::center() const
{
    return (m_box->AA() + m_box->BB()) * 0.5f;
}

bool BVHNode::hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const
{
    float hitTime;
    if (!m_box->hit(ray, hitTime))
        return false;

    bool lhsHit = m_left != nullptr && m_left->hit(ray, tMin, tMax, record);
    bool rhsHit = m_right != nullptr && m_right->hit(ray, tMin, tMax, record);

    return lhsHit || rhsHit;
}