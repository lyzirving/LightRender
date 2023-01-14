#ifndef GFX_DEVICE_H
#define GFX_DEVICE_H

class GfxDevice
{
public:
	static GfxDevice* get();

	static void init();
private:
	GfxDevice() {};
	virtual ~GfxDevice() = default;
};

#endif // !GFX_DEVICE_H
