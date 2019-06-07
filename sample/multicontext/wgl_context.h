#ifndef __WGL_CONTEXT_H__
#define __WGL_CONTEXT_H__

#include <Windows.h>
#include <glad/glad.h>
#include "wgl.h"
#include "wglext.h"

namespace el
{
    bool initializeWGL();
    bool loadGLFunctions();

    extern PFNWGLGETPROCADDRESSPROC wglGetProcAddress;
	extern PFNWGLMAKECURRENTPROC wglMakeCurrent;
	extern PFNWGLCREATECONTEXTPROC wglCreateContext;
	extern PFNWGLDELETECONTEXTPROC wglDeleteContext;
    extern PFNWGLGETCURRENTDCPROC wglGetCurrentDC;
    extern PFNWGLGETCURRENTCONTEXTPROC wglGetCurrentContext;
    extern PFNWGLSHARELISTSPROC wglShareLists;
}

#endif // __WGL_CONTEXT_H__
