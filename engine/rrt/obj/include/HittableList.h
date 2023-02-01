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

	void add(const std::shared_ptr<Hittable>& hittable) { m_list.emplace_back(hittable); }

	virtual void hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const override;

private:
	std::vector<std::shared_ptr<Hittable>> m_list;
};

#endif
