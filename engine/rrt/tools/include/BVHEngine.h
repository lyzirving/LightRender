#ifndef BVH_ENGINE_H
#define BVH_ENGINE_H

#include <memory>

class BVHNode;
class Hittable;
class HittableList;

class BVHEngine
{
public:
	static std::shared_ptr<BVHNode> simpleBuild(const std::shared_ptr<HittableList> &list);
	static std::shared_ptr<BVHNode> sahBuild(const std::shared_ptr<HittableList>& list, int limit);

private:
	static bool boxCmp(int axis, const std::shared_ptr<Hittable>& lhs, const std::shared_ptr<Hittable>& rhs);
	static bool boxCmpX(const std::shared_ptr<Hittable>& lhs, const std::shared_ptr<Hittable>& rhs);
	static bool boxCmpY(const std::shared_ptr<Hittable>& lhs, const std::shared_ptr<Hittable>& rhs);
	static bool boxCmpZ(const std::shared_ptr<Hittable>& lhs, const std::shared_ptr<Hittable>& rhs);

	static bool centerCmp(int axis, const std::shared_ptr<Hittable>& lhs, const std::shared_ptr<Hittable>& rhs);
	static bool centerCmpX(const std::shared_ptr<Hittable>& lhs, const std::shared_ptr<Hittable>& rhs);
	static bool centerCmpY(const std::shared_ptr<Hittable>& lhs, const std::shared_ptr<Hittable>& rhs);
	static bool centerCmpZ(const std::shared_ptr<Hittable>& lhs, const std::shared_ptr<Hittable>& rhs);

	static void sahLowestCost(const std::shared_ptr<HittableList>& list, int l, int r, float& lowCost, int& lowAxis, int& lowSplit);

	static void simpleBuild(const std::shared_ptr<BVHNode>& parent, const std::shared_ptr<HittableList>& list, int left, int right);
	static void sahBuild(const std::shared_ptr<BVHNode>& parent, const std::shared_ptr<HittableList>& list, int left, int right, int limit);

	BVHEngine() {}
	~BVHEngine() = default;
};

#endif