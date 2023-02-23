#ifndef GFX_SHADER_MGR_H
#define GFX_SHADER_MGR_H

#include <map>
#include <memory>

#include "GfxDef.h"

class GfxShader;

class GfxShaderMgr
{
public:
    static GfxShaderMgr* get();
    static void release();

    void init();
    const std::shared_ptr<GfxShader>& getShader(GfxShaderType type);

private:
    GfxShaderMgr();
    ~GfxShaderMgr();

    static const char* type2Str(GfxShaderType type);

    std::map<std::string, std::shared_ptr<GfxShader>> m_shaders;
};

#endif // !GFX_SHADER_MGR_H
