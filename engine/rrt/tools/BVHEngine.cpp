#include <cassert>
#include <algorithm>
#include <limits>

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

#ifdef INF
#undef INF
#endif
#define INF 114514.0

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

bool BVHEngine::centerCmp(int axis, const std::shared_ptr<Hittable>& lhs, const std::shared_ptr<Hittable>& rhs)
{
	if (axis == 0)
	{
		return lhs->center().x < rhs->center().x;
	}
	else if (axis == 1)
	{
		return lhs->center().y < rhs->center().y;
	}
	else
	{
		return lhs->center().z < rhs->center().z;
	}
}

bool BVHEngine::centerCmpX(const std::shared_ptr<Hittable>& lhs, const std::shared_ptr<Hittable>& rhs)
{
	return centerCmp(0, lhs, rhs);
}

bool BVHEngine::centerCmpY(const std::shared_ptr<Hittable>& lhs, const std::shared_ptr<Hittable>& rhs)
{
	return centerCmp(1, lhs, rhs);
}

bool BVHEngine::centerCmpZ(const std::shared_ptr<Hittable>& lhs, const std::shared_ptr<Hittable>& rhs)
{
	return centerCmp(2, lhs, rhs);
}

std::shared_ptr<BVHNode> BVHEngine::simpleBuild(const std::shared_ptr<HittableList>& list)
{
	std::shared_ptr<BVHNode> root = std::make_shared<BVHNode>();
	simpleBuild(root, list, 0, list->size() - 1);
	return root;
}

std::shared_ptr<BVHNode> BVHEngine::sahBuild(const std::shared_ptr<HittableList>& list, int limit)
{
	std::shared_ptr<BVHNode> root = std::make_shared<BVHNode>();
	sahBuild(root, list, 0, list->size() - 1, limit);
	return root;
}

void BVHEngine::simpleBuild(const std::shared_ptr<BVHNode>& parent, const std::shared_ptr<HittableList>& list, int left, int right)
{
	if (left > right)
	{
		LOG_ERR("err, left[%d] is larger than right[%d]", left, right);
		return;
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

void BVHEngine::sahBuild(const std::shared_ptr<BVHNode>& parent, const std::shared_ptr<HittableList>& list, int left, int right, int limit)
{
	if (left > right)
	{
		LOG_ERR("err, left[%d] is larger than right[%d]", left, right);
		return;
	}
	int spanLen = right - left + 1;

	// make leaf node
	if (spanLen <= limit)
	{
		if (spanLen == 1)
		{
			parent->m_left = parent->m_right = list->at(left);
		}
		else if(spanLen == 2)
		{
			// two leaf node, randomly select an axis to compare
			int randomAxis = GfxLib::randomInt(0, 2);
			if (boxCmp(randomAxis, list->at(left), list->at(left + 1)))
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
			parent->m_left = parent->m_right = list->subgroup(left, spanLen);
		}
	}
	else
	{
		// find the split position, axis for lowest cost
		int axis = 0;
		float cost = INF;
		int split = (left + right) / 2;

		sahLowestCost(list, left, right, cost, axis, split);

		// sort by the axis with the lowest cost
		if (axis == 0) std::sort(list->m_list.begin() + left, list->m_list.begin() + right + 1, centerCmpX);
		if (axis == 1) std::sort(list->m_list.begin() + left, list->m_list.begin() + right + 1, centerCmpY);
		if (axis == 2) std::sort(list->m_list.begin() + left, list->m_list.begin() + right + 1, centerCmpZ);

		// make middle node
		std::shared_ptr<BVHNode> leftParent = std::make_shared<BVHNode>();
		std::shared_ptr<BVHNode> rightParent = std::make_shared<BVHNode>();

		parent->m_left = leftParent;
		parent->m_right = rightParent;

		sahBuild(leftParent, list, left, split, limit);
		sahBuild(rightParent, list, split + 1, right, limit);
	}

	AABB leftChildBox, rightChildBox;
	if (!parent->m_left->boundingBox(leftChildBox) || !parent->m_right->boundingBox(rightChildBox))
		LOG_ERR("fail to get bounding box for left child and right child");

	*parent->m_box = AABB::surroundingBox(leftChildBox, rightChildBox);
}

void BVHEngine::sahLowestCost(const std::shared_ptr<HittableList>& list, int l, int r, float& lowCost, int& lowAxis, int& lowSplit)
{
	for (int axisInd = 0; axisInd < 3; ++axisInd)
	{
		// sort the triangles by certain axis
		if (axisInd == 0) std::sort(list->m_list.begin() + l, list->m_list.begin() + r + 1, centerCmpX);
		if (axisInd == 1) std::sort(list->m_list.begin() + l, list->m_list.begin() + r + 1, centerCmpY);
		if (axisInd == 2) std::sort(list->m_list.begin() + l, list->m_list.begin() + r + 1, centerCmpZ);

		std::vector<glm::vec3> leftMin(r - l + 1, glm::vec3(INF));
		std::vector<glm::vec3> leftMax(r - l + 1, glm::vec3(-INF));

		for (int i = l; i <= r; ++i)
		{
			AABB box;
			list->at(i)->boundingBox(box);

			int bias = ((i == l) ? 0 : 1);

			leftMin[i - l] = glm::min(leftMin[i - l - bias], box.AA());
			leftMax[i - l] = glm::max(leftMax[i - l - bias], box.BB());
		}

		std::vector<glm::vec3> rightMin(r - l + 1, glm::vec3(INF));
		std::vector<glm::vec3> rightMax(r - l + 1, glm::vec3(-INF));
		for (int i = r; i >= l; --i)
		{
			AABB box;
			list->at(i)->boundingBox(box);

			int bias = ((i == r) ? 0 : 1);

			rightMin[i - l] = glm::min(rightMin[i - l + bias], box.AA());
			rightMax[i - l] = glm::max(rightMax[i - l + bias], box.BB());
		}

		int split = l;
		float cost = INF;
		// split from l to r - 1 to calculate cost and split position
		for (int i = l; i <= r - 1; ++i)
		{
			// left side[l, i]
			glm::vec3 leftAA = leftMin[i - l];
			glm::vec3 leftBB = leftMax[i - l];
			glm::vec3 diff = leftBB - leftAA;
			float leftArea = 2 * (diff.x * diff.y + diff.x * diff.z + diff.y * diff.z);
			float leftCost = leftArea * float(i - l + 1);

			// right side[i + 1, r]
			glm::vec3 rightAA = rightMin[i - l + 1];
			glm::vec3 rightBB = rightMax[i - l + 1];
			diff = rightBB - rightAA;
			float rightArea = 2 * (diff.x * diff.y + diff.x * diff.z + diff.y * diff.z);
			float rightCost = rightArea * float(r - i);

			float totalCost = leftCost + rightCost;
			if (totalCost < cost)
			{
				cost = totalCost;
				split = i;
			}
		}

		if (cost < lowCost)
		{
			lowCost = cost;
			lowSplit = split;
			lowAxis = axisInd;
		}
	}
}