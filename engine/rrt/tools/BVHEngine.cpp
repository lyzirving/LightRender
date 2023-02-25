#include <cassert>
#include <algorithm>

#include "BVHEngine.h"
#include "BVHNode.h"
#include "HittableList.h"
#include "AABB.h"

#include "GfxLib.h"

#include "Logger.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "BVHEngine"

bool BVHEngine::boxCmp(int axis, const std::shared_ptr<Hittable>& lhs, const std::shared_ptr<Hittable>& rhs)
{
	AABB lhsBox, rhsBox;

	if (!lhs->boundingBox(lhsBox) || !rhs->boundingBox(rhsBox))
		LOG_ERR("fail to find bounding box");

	if (axis == 0)
	{
		return lhsBox.AA().x < rhsBox.AA().x;
	}
	else if (axis == 1)
	{
		return lhsBox.AA().y < rhsBox.AA().y;
	}
	else
	{
		return lhsBox.AA().z < rhsBox.AA().z;
	}
}

bool BVHEngine::boxCmpX(const std::shared_ptr<Hittable>& lhs, const std::shared_ptr<Hittable>& rhs)
{
	return boxCmp(0, lhs, rhs);
}

bool BVHEngine::boxCmpY(const std::shared_ptr<Hittable>& lhs, const std::shared_ptr<Hittable>& rhs)
{
	return boxCmp(1, lhs, rhs);
}

bool BVHEngine::boxCmpZ(const std::shared_ptr<Hittable>& lhs, const std::shared_ptr<Hittable>& rhs)
{
	return boxCmp(2, lhs, rhs);
}

std::shared_ptr<BVHNode> BVHEngine::simpleBuild(const std::shared_ptr<HittableList>& list)
{
	std::shared_ptr<BVHNode> root = std::make_shared<BVHNode>();
	simpleBuild(root, list, 0, list->size() - 1);
	return root;
}

void BVHEngine::simpleBuild(const std::shared_ptr<BVHNode>& parent, const std::shared_ptr<HittableList>& list, int left, int right)
{
	if (left > right)
	{
		LOG_ERR("err, left[%d] is larger than right[%d]", left, right);
		assert(0);
	}
	int spanLen = right - left + 1;
	int axis = GfxLib::randomInt(0, 2);
	auto cmp = axis == 0 ? boxCmpX : (axis == 1 ? boxCmpY : boxCmpZ) ;

	if (spanLen == 1)
	{
		// one leaf node, give the same hittable for both nodes
		parent->m_left = parent->m_right = list->at(left);
	}
	else if (spanLen == 2)
	{
		// two leaf node
		if (boxCmp(axis, list->at(left), list->at(left + 1)))
		{
			parent->m_left = list->at(left);
			parent->m_right = list->at(left + 1);
		}
		else
		{
			parent->m_left = list->at(left + 1);
			parent->m_right = list->at(left);
		}
	}
	else
	{
		std::sort(list->m_list.begin() + left, list->m_list.begin() + right + 1, cmp);
		// make middle node
		std::shared_ptr<BVHNode> leftParent = std::make_shared<BVHNode>();
		std::shared_ptr<BVHNode> rightParent = std::make_shared<BVHNode>();

		parent->m_left = leftParent;
		parent->m_right = rightParent;

		int mid = left + spanLen / 2 - 1;
		simpleBuild(leftParent, list, left, mid);
		simpleBuild(rightParent, list, mid + 1, right);
	}

	AABB leftChildBox, rightChildBox;
	if (!parent->m_left->boundingBox(leftChildBox) || !parent->m_right->boundingBox(rightChildBox))
		LOG_ERR("fail to get bounding box for left child and right child");

	*parent->m_box = AABB::surroundingBox(leftChildBox, rightChildBox);
}