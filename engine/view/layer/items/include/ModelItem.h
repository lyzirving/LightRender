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
    ModelItem(const char* name);
    virtual ~ModelItem();

    virtual void draw(const std::shared_ptr<ViewTransform>& trans) override;

protected:
    void loadModel();
    std::vector<Texture> loadTexture(aiMaterial* mt, aiTextureType type, uint8_t texType);
    void processNode(aiNode* node, const aiScene* scene);
    std::shared_ptr<GfxMesh> processMesh(aiMesh* mesh, const aiScene* scene);
    void updateMinMax(const glm::vec3& pos);

    std::string m_name, m_srcRoot;
    uint32_t m_meshInd;
    std::vector<std::shared_ptr<GfxMesh>> m_mesh;

    glm::vec3 m_minPos, m_maxPos;
};

#endif // !MODEL_ITEM_H

