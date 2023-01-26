#ifndef MODEL_ITEM_H
#define MODEL_ITEM_H

#include <string>

#include "LayerItem.h"

class aiScene;
class aiNode;
class aiMesh;
struct aiMaterial;
enum aiTextureType;

class ModelItem : public LayerItem
{
public:
    ModelItem(const char* name);
    virtual ~ModelItem();

    virtual void draw(const std::shared_ptr<ViewTransform>& trans) override;

protected:
    void loadModel();

private:
    std::string m_name;
};

#endif // !MODEL_ITEM_H

