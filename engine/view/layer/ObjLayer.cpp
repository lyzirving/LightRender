#include "ObjLayer.h"
#include "LayerItem.h"

ObjLayer::ObjLayer(LayerOrder order) : Layer(LayerType::OBJ_LAYER, order)
{
    ObjLayer::createItems();
}

ObjLayer::~ObjLayer() = default;

void ObjLayer::createItems()
{
   
}

void ObjLayer::update(const std::shared_ptr<ViewTransform>& trans)
{
    for (auto& item : m_items)
    {
        item->draw(trans);
    }
}