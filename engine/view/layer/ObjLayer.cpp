#include "ObjLayer.h"
#include "ModelItem.h"

ObjLayer::ObjLayer(LayerOrder order) : Layer(LayerType::LAYER_OBJ, order)
{
    ObjLayer::createItems();
}

ObjLayer::~ObjLayer() = default;

void ObjLayer::createItems()
{
    std::shared_ptr<LayerItem> modelItem = std::make_shared<ModelItem>("Marry");

    m_items.push_back(std::move(modelItem));
}

void ObjLayer::update(const std::shared_ptr<ViewTransform>& trans)
{
    for (auto& item : m_items)
    {
        item->draw(trans);
    }
}