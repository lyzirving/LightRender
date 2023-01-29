#ifndef RRT_RENDER_H
#define RRT_RENDER_H

#include "Render.h"

class RrtRender : public Render
{
public:
	RrtRender();

	virtual ~RrtRender();

protected:
	virtual void createLayers() override;
};

#endif // !RRT_RENDER_H
