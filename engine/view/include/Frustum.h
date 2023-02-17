#ifndef FRUSTUM_H
#define FRUSTUM_H

#include <atomic>
#include <glm/glm.hpp>
#include <list>

class Frustum
{
public:
	Frustum(float fov = 45.f, float aspect = 1.f, float near = 0.1, float far = 10.f);
	virtual ~Frustum();

	inline float near() { return m_near; }
	inline float far() { return m_far; }
	inline float fov() { return m_fov; }
	inline float aspect() { return m_aspect; };
	inline bool isChanged() { return m_change.load(); }

	const glm::mat4& getProjectMat();
	void restoreState();
	void popState();
	void setFov(float fov);
	void setAspect(float aspect);
	void setNearFar(float near, float far);

private:
	struct FrustumState
	{
		float m_fov, m_aspect;
		float m_near, m_far;

		FrustumState() : m_fov(0.f), m_aspect(0.f), m_near(0.f), m_far(0.f) {}
		FrustumState(float fov, float aspect, float near, float far) : m_fov(fov), m_aspect(aspect),
			m_near(near), m_far(far) {}
	};
	void calcProjectMat();

	float m_fov, m_aspect;
	float m_near, m_far;
	std::atomic_bool m_change;
	glm::mat4 m_projectMat;
	std::list<FrustumState> m_stateStack;
};

#endif // !FRUSTUM_H

