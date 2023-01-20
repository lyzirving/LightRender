#include <GL/glew.h>

#include "ClearLayer.h"

ClearLayer::ClearLayer(LayerOrder order) : Layer(LayerType::BASIC, order), 
                                           m_clearColor(0xffffffff)
{
}

ClearLayer::~ClearLayer() = default;

void ClearLayer::createItems() {}

void ClearLayer::update(const std::shared_ptr<ViewTransform>& trans)
{
    glClearColor(R_COMP(m_clearColor), G_COMP(m_clearColor), B_COMP(m_clearColor), A_COMP(m_clearColor));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}