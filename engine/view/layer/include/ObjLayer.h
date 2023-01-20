#ifndef OBJ_LAYER_H
#define OBJ_LAYER_H

#include "Layer.h"

class ObjLayer : public Layer
{
public:
    ObjLayer(LayerOrder order = LayerOrder::MID);
    virtual ~ObjLayer();

    virtual void update(const std::shared_ptr<ViewTransform>& trans) override;

protected:
    virtual void createItems() override;
};

#endif // !OBJ_LAYER_H

