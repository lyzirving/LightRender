#include <cassert>
#include <limits>

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
	if (m_list.empty())
		return false;

	AABB tmpBox;
	bool first{true};
	for (const std::shared_ptr<Hittable>& item : m_list)
	{
		if (!item->boundingBox(tmpBox)) return false;

		if (first)
		{
			first = false;
			box = tmpBox;
		}
		else
		{
			box = AABB::surroundingBox(box, tmpBox);
		}
	}
	return true;
}

glm::vec3 HittableList::center() const
{
	AABB box;
	if (!boundingBox(box))
	{
		LOG_ERR("fail to find bounding box");
		assert(0);
	}
	return (box.AA() + box.BB()) * 0.5f;
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

bool HittableList::empty() const
{
	return m_list.empty();
}

std::shared_ptr<HittableList> HittableList::subgroup(int start, int len) const
{
	std::shared_ptr<HittableList> list = std::make_shared<HittableList>();
	list->m_list.assign(m_list.begin() + start, m_list.begin() + start + len);
	return list;
}

int HittableList::size() const
{
	return m_list.size();
}