#ifndef MODEL_ITEM_H
#define MODEL_ITEM_H

#include <string>
#include <vector>

#include "LayerItem.h"

class aiScene;
class aiNode;
class aiMesh;
struct aiMaterial;
enum aiTextureType;

class GfxMesh;
struct Texture;

class ModelItem : public LayerItem
{
public:
    ModelItem(const char* name, bool fCenter = false, bool fScale = false);
    virtual ~ModelItem();

    virtual void draw(const std::shared_ptr<ViewTransform>& trans) override;

protected:
    void loadModel();
    std::vector<Texture> loadTexture(aiMaterial* mt, aiTextureType type, uint8_t texType);
    void processNode(aiNode* node, const aiScene* scene);
    std::shared_ptr<GfxMesh> processMesh(aiMesh* mesh, const aiScene* scene);

    std::string m_name, m_srcRoot;
    uint32_t m_meshInd;
    std::vector<std::shared_ptr<GfxMesh>> m_mesh;
};

#endif // !MODEL_ITEM_H

