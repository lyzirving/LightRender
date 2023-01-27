#ifndef GFX_HELPER_H
#define GFX_HELPER_H

#include <string>
#include <unordered_map>

struct Texture;

class GfxHelper
{
public:
	static GfxHelper* get();
	static void release();
	static uint32_t colorFormat(int component);
	static bool checkGlErr(const char* fmt, ...);

	bool loadTex(const std::string& path, uint32_t& texId, 
		         int32_t& width, int32_t& height, int32_t& channel);

private:
	GfxHelper();
	~GfxHelper();

	std::unordered_map<std::string, Texture> m_texMap;
};

#endif // !GFX_HELPER_H

