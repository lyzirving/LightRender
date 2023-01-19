#ifndef LAYER_ITEM_H
#define LAYER_ITEM_H

#include <memory>
#include <glm/glm.hpp>

class ViewTransform;

class LayerItem
{
public:
    LayerItem() : m_modelMat(1.f), m_transMat(1.f), m_scaleMat(1.f), m_rotateMat(1.f)
    {
    }

    virtual ~LayerItem() = default;

    virtual void draw(const std::shared_ptr<ViewTransform>& trans) = 0;
    virtual void release() {}

protected:
    glm::mat4 m_modelMat;
    glm::mat4 m_transMat, m_scaleMat, m_rotateMat;
};

#endif // !LAYER_ITEM_H

