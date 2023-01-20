#ifndef CLEAR_LAYER_H
#define CLEAR_LAYER_H

#include "Layer.h"

class ViewTransform;

class ClearLayer : public Layer
{
public:
    ClearLayer(LayerOrder order = LayerOrder::LOW);
    virtual ~ClearLayer();

    virtual void update(const std::shared_ptr<ViewTransform>& trans) override;

protected:
    virtual void createItems() override;

private:
    uint32_t m_clearColor;
};

#endif // !CLEAR_LAYER_H

