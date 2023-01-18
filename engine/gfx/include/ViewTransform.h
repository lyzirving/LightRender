#ifndef VIEW_TRANSFORM_H
#define VIEW_TRANSFORM_H

#include <memory>
#include <glm/glm.hpp>

class Camera;

class ViewTransform
{
public:
	ViewTransform();
	virtual ~ViewTransform();

	const glm::mat4& getViewMat();
	const glm::mat4& getProjectMat();

	// const T*
	inline const std::shared_ptr<Camera>& camera() { return m_camera; }

private:
	std::shared_ptr<Camera> m_camera;
};

#endif // !VIEW_TRANSFORM_H

