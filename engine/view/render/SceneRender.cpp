#include "SceneRender.h"
#include "ClearLayer.h"
#include "ObjLayer.h"

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