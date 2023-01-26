#ifndef ASSETS_MGR_H
#define ASSETS_MGR_H

#include <string>

enum PIC_TYPE : uint8_t
{
    PNG,
    JPG
};

class AssetsMgr
{
public:
    static AssetsMgr* get();

    static std::string getObj(const std::string& name);
    static std::string getTexture(const std::string& name, PIC_TYPE type = PIC_TYPE::PNG);
    static std::string getVert(const std::string& name);
    static std::string getFrag(const std::string& name);

protected:
    enum SRC_TYPE : uint8_t
    {
        MODEL,
        TEXTURE,
        SHADER
    };

private:
    AssetsMgr();
    ~AssetsMgr();

    static std::string getPath(const std::string& path, SRC_TYPE type);
};


#endif // !ASSETS_MGR_H

