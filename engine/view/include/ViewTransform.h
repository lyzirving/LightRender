#ifndef VIEW_TRANSFORM_H
#define VIEW_TRANSFORM_H

#include <memory>
#include <glm/glm.hpp>

class Camera;
class Frustum;
class Viewport;

class ViewTransform
{
public:
	ViewTransform();
	virtual ~ViewTransform();

	const glm::mat4& getViewMat();
	const glm::mat4& getProjectMat();

	// const T*
	inline const std::shared_ptr<Camera>& camera() { return m_camera; }
	inline const std::shared_ptr<Frustum>& frustum() { return m_frustum; }
	inline const std::shared_ptr<Viewport>& viewport() { return m_viewport; }

	void setViewport(int32_t x, int32_t y, int32_t width, int32_t height);

private:
	std::shared_ptr<Camera> m_camera;
	std::shared_ptr<Frustum> m_frustum;
	std::shared_ptr<Viewport> m_viewport;
};

#endif // !VIEW_TRANSFORM_H

