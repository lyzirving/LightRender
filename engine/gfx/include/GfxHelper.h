#ifndef GFX_HELPER_H
#define GFX_HELPER_H

#include <string>
#include <unordered_map>

struct GfxTexture;

class GfxHelper
{
public:
	static GfxHelper* get();

	static uint32_t createProgram(const char* vert = nullptr, const char* frag = nullptr, 
		                          const char* geo = nullptr);
	static void release();
	static uint32_t colorFormat(int component);
	static bool checkGlErr(const char* fmt, ...);

	bool loadTex(const std::string& path, uint32_t& texId, 
		         int32_t& width, int32_t& height, int32_t& channel);

private:
	GfxHelper();
	~GfxHelper();

	static uint32_t createShader(uint32_t type, const char* source);
	static void logShaderInfo(uint32_t shader);

	std::unordered_map<std::string, GfxTexture> m_texMap;
};

#endif // !GFX_HELPER_H

