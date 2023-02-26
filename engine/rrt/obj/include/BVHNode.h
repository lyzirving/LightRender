#ifndef BVH_NODE_H
#define BVH_NODE_H

#include "HittableList.h"

class BVHNode : public Hittable
{
public:
	friend class BVHEngine;

	BVHNode();
	virtual ~BVHNode();

	virtual bool boundingBox(AABB& box) const override;
	virtual glm::vec3 center() const override;
	virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const override;

private:
	std::shared_ptr<AABB> m_box;
	/*
	* left and right are the children of this node.
	* left and right can be BVHNode, which is a middle node.
	* They can also be a sphere, hittable list or other hittable object, which is a leaf node.
	*/
	std::shared_ptr<Hittable> m_left, m_right;
};

#endif