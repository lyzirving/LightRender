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

private:
	static bool boxCmp(int axis, const std::shared_ptr<Hittable>& lhs, const std::shared_ptr<Hittable>& rhs);
	static bool boxCmpX(const std::shared_ptr<Hittable>& lhs, const std::shared_ptr<Hittable>& rhs);
	static bool boxCmpY(const std::shared_ptr<Hittable>& lhs, const std::shared_ptr<Hittable>& rhs);
	static bool boxCmpZ(const std::shared_ptr<Hittable>& lhs, const std::shared_ptr<Hittable>& rhs);
	static void simpleBuild(const std::shared_ptr<BVHNode>& parent, const std::shared_ptr<HittableList>& list, int left, int right);

	BVHEngine() {}
	~BVHEngine() = default;
};

#endif