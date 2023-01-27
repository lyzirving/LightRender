#ifndef GFX_MESH_H
#define GFX_MESH_H

#include <memory>
#include <string>
#include <vector>

class Shader;

struct Vertex;
struct Texture;
struct Material;

class GfxMesh
{
public:
    GfxMesh();
    GfxMesh(const char* name);

    virtual ~GfxMesh();
    virtual void bind(bool force);
    virtual void draw(const std::shared_ptr<Shader>& shader);

    inline const std::shared_ptr<Material>& getMaterial() { return m_material; }
    inline const std::string& getName() { return m_name; }
    std::string getMeshInfo();

    void pushBackVert(Vertex &&vert);
    void pushBackIndices(uint32_t ind);
    void pushBackTexVec(const std::vector<Texture> &texVec);

protected:
    virtual void createMem();
    virtual void freeMem();

    bool m_initialized;
    std::string m_name;
    uint32_t m_vao, m_vbo, m_ebo;

    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;
    std::vector<Texture> m_textures;

    std::shared_ptr<Material> m_material;
};

#endif // !GFX_MESH_H
