#ifndef GFX_CONTEXT_H
#define GFX_CONTEXT_H

#include <Windows.h>

struct WinInfo
{
	int   pixFmt;
	// handle of window
	HWND  hWnd;
	// context of device
	HDC	  hDc;
	// context of opengl
	HGLRC hRc;

	WinInfo() : pixFmt(0), hWnd(0), hDc(0), hRc(0) {}
};

class GfxContext
{
public:
	GfxContext();
	~GfxContext();

	void bind(HWND hWnd);
	bool makeCurrent();
	void release();
	bool swapBuf();

protected:
	void setGLPixFmt();

	WinInfo m_winInfo;

};

#endif // !GFX_CONTEXT_H

