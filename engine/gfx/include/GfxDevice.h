#ifndef GFX_DEVICE_H
#define GFX_DEVICE_H

class GfxDevice
{
public:
	static GfxDevice* get();

	static void init();
	static void setViewport(int l, int b, int width, int height);
private:
	GfxDevice() {};
	virtual ~GfxDevice() = default;
};

#endif // !GFX_DEVICE_H
