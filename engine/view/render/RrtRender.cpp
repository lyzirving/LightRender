#include <GL/glew.h>

#include "RrtRender.h"
#include "RrtCanvasLayer.h"

RrtRender::RrtRender() : Render("Realtime Ray Tracing")
{
}

RrtRender::~RrtRender() = default;

void RrtRender::createLayers()
{
	std::shared_ptr<Layer> casLayer = std::make_shared<RrtCanvasLayer>();

	addLayer(casLayer);
}