#ifndef GFX_CONTEXT_H
#define GFX_CONTEXT_H

#include <Windows.h>

class GfxContext
{
public:
	GfxContext();
	~GfxContext();

	void bind(HWND wnd);
	bool makeCurrent();
	void release();
	bool swapBuf();

protected:
	struct WinInfo
	{
		int   pixFmt;
		// handle of window
		HWND  hdl_wnd;
		// context of device
		HDC	  hdl_dc;
		// context of opengl
		HGLRC hdl_rc;

		WinInfo() : pixFmt(0), hdl_wnd(nullptr), hdl_dc(nullptr), hdl_rc(nullptr) {}
	};

	void setGLPixFmt();

	WinInfo m_winInfo;

};

#endif // !GFX_CONTEXT_H

