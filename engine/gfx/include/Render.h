#ifndef RENDER_H
#define RENDER_H

#include <vector>
#include <map>
#include <memory>
#include <string>

#include "Layer.h"

class ViewTransform;

class Render
{
public:
	Render();
	Render(const char *name);

	virtual ~Render();
	virtual void release();
	virtual void init();
	virtual void update() final;

	void addLayer(const std::shared_ptr<Layer>& layer);
	void removeLayer(const std::shared_ptr<Layer>& layer);
	void setViewport(int l, int b, int width, int height);

protected:
	struct
	{
		bool operator() (const std::shared_ptr<Layer>& lhs, const std::shared_ptr<Layer>& rhs)
		{
			if (!lhs) return false;
			if (!rhs) return true;
			return lhs->order() <= rhs->order();
		}
	} LayerSorter;

	virtual void createLayers() = 0;

	virtual void preRender();
	virtual void onRender();
	virtual void postRender();

	void sortLayer();

	std::string m_name;
	std::shared_ptr<ViewTransform> m_transform;

	std::vector<std::shared_ptr<Layer>> m_layers;
	std::map<LayerKey, std::shared_ptr<Layer>> m_existence;
};

#endif // !RENDER_H

