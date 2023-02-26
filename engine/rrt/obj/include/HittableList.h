#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <vector>
#include <memory>

#include "Hittable.h"

class HittableList : public Hittable
{
public:
	friend class BVHEngine;

	HittableList();
	virtual ~HittableList();

	inline void add(std::shared_ptr<Hittable>&& hittable) { m_list.push_back(std::move(hittable)); }
	inline void add(const std::shared_ptr<Hittable>& hittable) { m_list.push_back(hittable); }

	const std::shared_ptr<Hittable>& at(int index) const;
	bool empty() const;
	std::shared_ptr<HittableList> subgroup(int start, int len) const;
	int size() const;

	virtual bool boundingBox(AABB& box) const override;
	virtual glm::vec3 center() const override;
	virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const override;

private:
	std::vector<std::shared_ptr<Hittable>> m_list;
};

#endif
