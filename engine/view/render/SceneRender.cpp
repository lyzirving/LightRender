#include <Windows.h>
#include <direct.h>

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
	createLayers();
}

SceneRender::~SceneRender() = default;

void SceneRender::createLayers()
{
	std::shared_ptr<Layer> clearLayer = std::make_shared<ClearLayer>(LayerOrder::LOW);
	std::shared_ptr<Layer> objLayer = std::make_shared<ObjLayer>(LayerOrder::MID);

	addLayer(clearLayer);
	addLayer(objLayer);
	// get project root directory
	/*char buf[MAX_PATH];
	getcwd(buf, MAX_PATH);
	std::string path = buf;*/
}

void SceneRender::onRender()
{
	for (auto& layer : m_layers)
	{
		layer->update(m_transform);
	}
}