#ifndef RRT_COMPONENT_H
#define RRT_COMPONENT_H

#include <cstdint>

class Ray;

struct RrtCompArg
{
	float   argFloat;
	int32_t argInt32;
	void    *ptr;

	RrtCompArg() : argFloat(0.f)
		         , argInt32(0)
		         , ptr(nullptr)
	{}
};

enum RrtCompType : uint8_t
{
	RRT_TRANSFORM
};

class RrtComponent
{
public:
	RrtComponent() {};
	virtual ~RrtComponent() = default;

	virtual void process(Ray &ray, RrtCompArg&arg) = 0;
	virtual void postProcess(Ray& ray, RrtCompArg& arg) = 0;
};

#endif
