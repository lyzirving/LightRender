#ifndef GFX_MESH_H
#define GFX_MESH_H

#include <memory>
#include <string>
#include <vector>

class GfxShader;

struct GfxVertex;
struct GfxTexture;
struct GfxMaterial;

class GfxMesh
{
public:
    GfxMesh();
    GfxMesh(const char* name);

    virtual ~GfxMesh();
    virtual void bind(bool force);
    virtual void draw(const std::shared_ptr<GfxShader>& shader);

    inline const std::shared_ptr<GfxMaterial>& getMaterial() { return m_material; }
    inline const std::string& getName() { return m_name; }
    std::string getMeshInfo();

    void pushBackVert(GfxVertex &&vert);
    void pushBackIndices(uint32_t ind);
    void pushBackTexVec(const std::vector<GfxTexture> &texVec);

protected:
    virtual void createMem();
    virtual void freeMem();
    uint32_t getGlDrawMode();

    bool m_initialized;
    std::string m_name;
    uint32_t m_vao, m_vbo, m_ebo;

    std::vector<GfxVertex> m_vertices;
    std::vector<uint32_t> m_indices;
    std::vector<GfxTexture> m_textures;

    std::shared_ptr<GfxMaterial> m_material;

    uint8_t m_drawMode;
};

#endif // !GFX_MESH_H
