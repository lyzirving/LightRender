#ifndef SCENE_RENDER_H
#define SCENE_RENDER_H

#include "Render.h"

class SceneRender : public Render
{
public:
	SceneRender();

	virtual ~SceneRender();

protected:
	virtual void createLayers() override;
	virtual void onRender() override;
};

#endif // !SCENE_RENDER_H

