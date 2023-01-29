#ifndef RRT_CANVAS_LAYER_H
#define RRT_CANVAS_LAYER_H

#include "Layer.h"

class Shader;

class BVHBuilder;
class RrtTriBuf;
class RrtBVHBuf;

class RrtCanvasLayer : public Layer
{
public:
    RrtCanvasLayer(LayerOrder order = LayerOrder::LOW);
    virtual ~RrtCanvasLayer();

    virtual void update(const std::shared_ptr<ViewTransform>& trans) override;

protected:
    virtual void createItems() override;

private:
    void initMem();
    void drawCall();

    uint32_t m_vao, m_vbo, m_ebo;
    uint32_t m_bgColor;

    std::shared_ptr<Shader> m_shader;
    std::shared_ptr<BVHBuilder> m_BVHBuilder;

    std::shared_ptr<RrtTriBuf> m_triBuf;
    std::shared_ptr<RrtBVHBuf> m_BVHBuf;
};

#endif //RRT_CANVAS_LAYER_H