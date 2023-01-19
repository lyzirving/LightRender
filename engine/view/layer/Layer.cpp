#include "Layer.h"

Layer::Layer() : m_type(LayerType::BASIC), m_order(LayerOrder::LOW),
                 m_key(m_type, m_order), m_items()
{

}

Layer::Layer(LayerType type, LayerOrder order) : m_type(type), m_order(order),
                                                 m_key(type, order), m_items()
{

}

Layer::~Layer()
{
    auto itr = m_items.begin();
    while (itr != m_items.end())
    {
        (*itr).reset();
        itr = m_items.erase(itr);
    }
}