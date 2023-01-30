#include <GL/glew.h>

#include "RrtRender.h"
#include "RrtCanvasLayer.h"
#include "ViewTransform.h"
#include "Camera.h"

RrtRender::RrtRender() : Render("Realtime Ray Tracing")
{
}

RrtRender::~RrtRender() = default;

void RrtRender::createLayers()
{
	std::shared_ptr<Layer> casLayer = std::make_shared<RrtCanvasLayer>();

	addLayer(casLayer);
}

void RrtRender::preRender()
{
	Render::preRender();

	const std::shared_ptr<Camera>& cam = m_transform->camera();
	cam->setPosition(90.f, 60.f, 4.f);
}