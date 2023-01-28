#ifndef SHADER_MGR_H
#define SHADER_MGR_H

#include <map>
#include <memory>

#include "GfxDef.h"

class Shader;

class ShaderMgr
{
public:
    static ShaderMgr* get();
    static void release();

    void init();
    const std::shared_ptr<Shader>& getShader(ShaderType type);

private:
    ShaderMgr();
    ~ShaderMgr();

    static const char* type2Str(ShaderType type);

    std::map<std::string, std::shared_ptr<Shader>> m_shaders;
};

#endif // !SHADER_MGR_H
