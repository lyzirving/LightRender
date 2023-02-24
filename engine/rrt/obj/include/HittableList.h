#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <vector>
#include <memory>

#include "Hittable.h"

class HittableList : public Hittable
{
public:
	HittableList();
	virtual ~HittableList();

	inline void add(std::shared_ptr<Hittable>&& hittable) { m_list.push_back(std::move(hittable)); }
	inline void add(const std::shared_ptr<Hittable>& hittable) { m_list.push_back(hittable); }

	const std::shared_ptr<Hittable>& at(int index) const;

	virtual bool boundingBox(AABB& box) const override;
	virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const override;

private:
	std::vector<std::shared_ptr<Hittable>> m_list;
};

#endif
