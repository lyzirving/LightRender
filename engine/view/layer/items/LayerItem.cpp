#include <limits>

#include <glm/gtc/matrix_transform.hpp>

#include "LayerItem.h"
#include "ViewDef.h"

LayerItem::LayerItem(bool fCenter, bool fScale) : m_adjMat(1.f), 
                                                      m_modelMat(1.f), m_transMat(1.f), m_scaleMat(1.f), m_rotateMat(1.f),
                                                      m_adjFlag(0), m_minPos(FLT_MAX), m_maxPos(FLT_MIN)
{
    fitCenter(fCenter);
    fitScale(fScale);
}

LayerItem::~LayerItem() = default;

void LayerItem::computeAdjMat()
{
    bool adjCenter = (m_adjFlag & AdjFlag::ADJ_CENTER);
    bool adjScale = (m_adjFlag & AdjFlag::ADJ_SCALE);

    glm::mat4 transMat{ 1.f }, scaleMat{ 1.f };

    if (adjCenter)
    {
        glm::vec3 center = (m_maxPos + m_minPos) * 0.5f;
        transMat = glm::translate(transMat, -center);
    }

    if (adjScale)
    {
        glm::vec3 interval = m_maxPos - m_minPos;
        float scale = 2.f / std::max(std::max(interval.x, interval.y), interval.z);
        scaleMat = glm::scale(scaleMat, glm::vec3(scale));
    }

    m_adjMat = scaleMat * transMat;
}

void LayerItem::computeModelMat()
{
    m_modelMat = m_transMat * m_scaleMat * m_rotateMat;
}

void LayerItem::fitCenter(bool fit)
{
    if (fit)
    {
        m_adjFlag |= AdjFlag::ADJ_CENTER;
    }
    else
    {
        m_adjFlag &= ~AdjFlag::ADJ_CENTER;
    }
    computeAdjMat();
}

void LayerItem::fitScale(bool fit)
{
    if (fit)
    {
        m_adjFlag |= AdjFlag::ADJ_SCALE;
    }
    else
    {
        m_adjFlag &= ~AdjFlag::ADJ_SCALE;
    }
    computeAdjMat();
}

void LayerItem::release()
{
    //empty implementation here
}

void LayerItem::updateMinMax(const glm::vec3& pos)
{
	m_maxPos = glm::max(pos, m_maxPos);
	m_minPos = glm::min(pos, m_minPos);
}