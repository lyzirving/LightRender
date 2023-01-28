#include <GL/glew.h>

#include "SceneRender.h"

#include "ClearLayer.h"
#include "ObjLayer.h"
#include "ViewTransform.h"
#include "Camera.h"

#include "Logger.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "SceneRender"

SceneRender::SceneRender() : Render("Scene")
{
}

SceneRender::~SceneRender() = default;

void SceneRender::createLayers()
{
	std::shared_ptr<Layer> clearLayer = std::make_shared<ClearLayer>(LayerOrder::LOW);
	std::shared_ptr<Layer> objLayer = std::make_shared<ObjLayer>(LayerOrder::MID);

	addLayer(clearLayer);
	addLayer(objLayer);
}

void SceneRender::onRender()
{
	for (auto& layer : m_layers)
	{
		layer->update(m_transform);
	}
}

void SceneRender::preRender()
{
	Render::preRender();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	const std::shared_ptr<Camera> &cam = m_transform->camera();
	cam->setPosition(60.f, 45.f, 3.f);
}

void SceneRender::postRender()
{
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	Render::postRender();
}
