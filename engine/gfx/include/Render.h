#ifndef RENDER_H
#define RENDER_H

#include <glm/glm.hpp>
#include <memory>

class ViewLib;

class Render
{
public:
	Render();

	virtual ~Render();
	virtual void release();
	virtual void init();
	virtual void update() final;

	void setViewport(int l, int b, int width, int height);

protected:
	virtual void preRender();
	virtual void onRender();
	virtual void postRender();

	//x, y, z, w separately mean left, bottom, width and height
	glm::vec4 m_viewport;
	std::shared_ptr<ViewLib> m_viewLib;
};

#endif // !RENDER_H

