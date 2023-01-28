#ifndef LAYER_ITEM_H
#define LAYER_ITEM_H

#include <memory>
#include <glm/glm.hpp>

class ViewTransform;

class LayerItem
{
public:
    LayerItem(bool fCenter = false, bool fScale = false);

    virtual ~LayerItem();
    virtual void draw(const std::shared_ptr<ViewTransform>& trans) = 0;
    virtual void release();

    void fitCenter(bool fit);
    void fitScale(bool fit);

protected:
    void updateMinMax(const glm::vec3& pos);
    void computeAdjMat();
    void computeModelMat();

    glm::mat4 m_adjMat;
    glm::mat4 m_modelMat;
    glm::mat4 m_transMat, m_scaleMat, m_rotateMat;

    uint32_t m_adjFlag;
    glm::vec3 m_minPos, m_maxPos;
};

#endif // !LAYER_ITEM_H

