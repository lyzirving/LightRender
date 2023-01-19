#ifndef FRUSTUM_H
#define FRUSTUM_H

#include <atomic>
#include <glm/glm.hpp>

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
	void setFov(float fov);
	void setAspect(float aspect);
	void setNearFar(float near, float far);

private:
	void calcProjectMat();

	float m_fov, m_aspect;
	float m_near, m_far;
	std::atomic_bool m_change;
	glm::mat4 m_projectMat;
};

#endif // !FRUSTUM_H

