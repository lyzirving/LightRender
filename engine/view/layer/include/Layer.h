#ifndef LAYER_H
#define LAYER_H

#include <vector>
#include <memory>

#include "ViewDef.h"

struct LayerKey
{
    uint8_t type;
    uint8_t order;

    LayerKey() : type(0), order(0) {}
    LayerKey(uint8_t iType, uint8_t iOrder) : type(iType), order(iOrder) {}

    bool operator<(const LayerKey& other) const
    {
        if (this->type < other.type)
            return true;
        if (this->type > other.type)
            return false;

        // type is the same
        if (this->order < other.order)
            return true;
        else
            return false;
    }
};

class ViewTransform;
class LayerItem;

class Layer
{
public:
    Layer();
    Layer(LayerType type, LayerOrder order);

    virtual ~Layer();
    virtual void update(const std::shared_ptr<ViewTransform>& trans) = 0;

    inline LayerType type() { return m_type; }
    inline LayerOrder order() { return m_order; }
    inline const LayerKey& key() { return m_key; }

protected:
    virtual void createItems() = 0;

    LayerType m_type;
    LayerOrder m_order;
    LayerKey m_key;

    std::vector<std::shared_ptr<LayerItem>> m_items;
};


#endif // !LAYER_H


