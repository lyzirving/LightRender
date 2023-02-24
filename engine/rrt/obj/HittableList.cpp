#include <cassert>

#include "HittableList.h"
#include "AABB.h"

#include "Logger.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "HittableList"

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

const std::shared_ptr<Hittable>& HittableList::at(int index) const
{
	int size = m_list.size();
	if (index < 0 || index >= size)
	{
		LOG_ERR("invalid index[%d], size[%d]", index, size);
		assert(0);
	}
	return m_list[index];
}

bool HittableList::boundingBox(AABB& box) const
{
	return true;
}

bool HittableList::hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const
{
	float closest = tMax;
	HitRecord tmpRec;
	record = tmpRec;

	for (int i = 0; i < m_list.size(); ++i)
	{
		m_list[i]->hit(ray, tMin, closest, tmpRec);
		if (tmpRec.hit)
		{
			// it imitates the depth test
			closest = tmpRec.t;
			record = tmpRec;
			record.hitInd = i;
		}
	}
	return record.hit;
}