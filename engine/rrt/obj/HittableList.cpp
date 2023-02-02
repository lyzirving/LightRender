#include "HittableList.h"

HittableList::HittableList() : Hittable(), m_list()
{
}

HittableList::~HittableList()
{
	auto itr = m_list.begin();
	while (itr != m_list.end())
	{
		(*itr).reset();
		itr = m_list.erase(itr);
	}
}

void HittableList::hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const
{
	float closest = tMax;
	HitRecord tmpRec;

	for (auto& obj : m_list)
	{
		obj->hit(ray, tMin, closest, tmpRec);
		if (tmpRec.hit)
		{
			// it imitates the depth test
			closest = tmpRec.t;
			record = tmpRec;
		}
	}
}