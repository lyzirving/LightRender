#ifndef HITTABLE_H
#define HITTABLE_H

#include <memory>
#include <glm/glm.hpp>
#include <map>

#include "RrtComponent.h"

class Ray;
class RrtMaterial;
class AABB;

struct HitRecord
{
	glm::vec3 pt, n;
	float t;
	float u, v;
	int hitInd;
	bool hit, frontFace;
	
	HitRecord() : pt(0.f), n(0.f), t(0.f), u(0.f), v(0.f), hitInd(-1), hit(false), frontFace(false) {}
};

class Assemble
{
public:
	Assemble() : m_components() {}
	virtual ~Assemble()
	{
		auto itr = m_components.begin();
		while (itr != m_components.end())
		{
			itr->second.reset();
			itr = m_components.erase(itr);
		}
	}

	inline void addComp(RrtCompType type, const std::shared_ptr<RrtComponent> &comp)
	{
		auto itr = m_components.find(type);
		if (itr == m_components.end())
		{
			m_components.emplace(std::make_pair(type, comp));
		}
		else
		{
			itr->second = comp;
		}
	}

	inline void deleteComp(RrtCompType type)
	{
		auto itr = m_components.find(type);
		if (itr != m_components.end())
		{
			itr->second.reset();
			m_components.erase(itr);
		}
	}

	inline const std::shared_ptr<RrtComponent>& getComponent(RrtCompType type) const
	{
		auto itr = m_components.find(type);
		if (itr != m_components.end())
		{
			return itr->second;
		}
		return std::shared_ptr<RrtComponent>();
	};

	inline void processAllComp(Ray& ray, RrtCompArg& arg) const
	{
		auto itr = m_components.begin();
		while (itr != m_components.end())
		{
			itr->second->process(ray, arg);
			itr++;
		}
	};

	inline void postProcessAllComp(Ray& ray, RrtCompArg& arg) const
	{
		auto itr = m_components.begin();
		while (itr != m_components.end())
		{
			itr->second->postProcess(ray, arg);
			itr++;
		}
	};
protected:
	std::map<RrtCompType, std::shared_ptr<RrtComponent>> m_components;
};

class Hittable : public Assemble
{
public:
	Hittable() : Assemble(), m_matl(nullptr) {}
	Hittable(const std::shared_ptr<RrtMaterial> &material) : Assemble(), m_matl(material) {}
	virtual ~Hittable()  { m_matl.reset(); }

	inline const std::shared_ptr<RrtMaterial>& getMatl() const { return m_matl; };
	inline void setMatl(const std::shared_ptr<RrtMaterial>& matl) { m_matl = matl; }

	virtual bool boundingBox(AABB &box) const = 0;
	virtual glm::vec3 center() const = 0;
	virtual bool hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const = 0;

protected:
	std::shared_ptr<RrtMaterial> m_matl;
};

#endif
