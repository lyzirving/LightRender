#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>

class Ray
{
public:
	Ray();
	Ray(const Ray &other);
	Ray(const glm::vec3& orig, const glm::vec3& dir);
	Ray& operator=(const Ray& other);
	virtual ~Ray() = default;

	inline const glm::vec3& origin() const { return m_orig; }
	inline const glm::vec3& direction() const { return m_dir; }

	glm::vec3 at(float t) const;
	void setOrigin(const glm::vec3 &origin);
	void setDirection(const glm::vec3& direction);

private:
	glm::vec3 m_orig, m_dir;
};

#endif // !RAY_H
