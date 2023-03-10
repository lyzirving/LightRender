#include <cassert>
#include <algorithm>

#include <GL/glew.h>

#include "Render.h"

#include "ViewTransform.h"
#include "Viewport.h"

#include "Logger.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Render"

Render::Render() : m_name(), m_transform(new ViewTransform), m_layers(), m_existence() {}

Render::Render(const char* name) : m_name(name), m_transform(new ViewTransform), m_layers(), m_existence() {}

Render::~Render() 
{
	Render::release();
	m_transform.reset();
}

void Render::addLayer(const std::shared_ptr<Layer>& layer)
{
	if (m_existence.find(layer->key()) == m_existence.end())
	{
		m_layers.push_back(layer);
		m_existence.insert(std::pair<LayerKey, std::shared_ptr<Layer>>(layer->key(), layer));
		sortLayer();
	}
}

void Render::init()
{
	createLayers();
}

void Render::update() 
{
	preRender();
	onRender();
	postRender();
}

void Render::setViewport(int l, int b, int width, int height)
{
	m_transform->setViewport(l, b, width, height);
}

void Render::preRender() 
{
	const std::shared_ptr<Viewport> &viewport = m_transform->viewport();
	glViewport(viewport->x(), viewport->y(), viewport->width(), viewport->height());
}

void Render::onRender() 
{
	for (auto& layer : m_layers)
	{
		layer->update(m_transform);
	}
}

void Render::postRender() {}

void Render::release() 
{
	auto itr = m_layers.begin();
	while (itr != m_layers.end())
	{
		(*itr).reset();
		itr = m_layers.erase(itr);
	}

	auto mapItr = m_existence.begin();
	while (mapItr != m_existence.end())
	{
		mapItr->second.reset();
		mapItr = m_existence.erase(mapItr);
	}
}

void Render::removeLayer(const std::shared_ptr<Layer>& layer)
{
	if (!layer)
		return;

	bool found{ false };
	if (m_existence.find(layer->key()) != m_existence.end())
	{
		auto itr = m_layers.begin();
		while (itr != m_layers.end())
		{
			if ((*itr).get() == layer.get())
			{
				itr = m_layers.erase(itr);
				found = true;
				break;
			}
		}
		if (found)
		{
			auto mapItr = m_existence.find(layer->key());
			if (mapItr != m_existence.end())
			{
				mapItr->second.reset();
				mapItr = m_existence.erase(mapItr);
			}
		}
	}
	if (!found)
		LOG_INFO("fail to find layer(type[%u], order[%u])", layer->type(), layer->order());
}

void Render::sortLayer()
{
	if (!m_layers.empty())
	{
		std::sort(m_layers.begin(), m_layers.end(), LayerSorter);
	}
}