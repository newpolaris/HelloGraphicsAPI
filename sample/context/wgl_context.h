#ifndef __WGL_CONTEXT_H__
#define __WGL_CONTEXT_H__

#include <wingdi.h>
#include <Windows.h>

#include "GL/glext.h"
#include "GL/wglext.h"

namespace el
{
    bool initGL();
    bool initGLExtention();

    typedef BOOL (WINAPI * PFNWGLSWAPINTERVALEXTPROC)(int);
    typedef BOOL (WINAPI * PFNWGLGETPIXELFORMATATTRIBIVARBPROC)(HDC,int,int,UINT,const int*,int*);
    typedef const char* (WINAPI * PFNWGLGETEXTENSIONSSTRINGEXTPROC)(void);
    typedef const char* (WINAPI * PFNWGLGETEXTENSIONSSTRINGARBPROC)(HDC);
    typedef HGLRC (WINAPI * PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC,HGLRC,const int*);

    typedef HGLRC (WINAPI * PFN_wglCreateContext)(HDC);
    typedef BOOL (WINAPI * PFN_wglDeleteContext)(HGLRC);
    typedef PROC (WINAPI * PFN_wglGetProcAddress)(LPCSTR);
    typedef HDC (WINAPI * PFN_wglGetCurrentDC)(void);
    typedef HGLRC (WINAPI * PFN_wglGetCurrentContext)(void);
    typedef BOOL (WINAPI * PFN_wglMakeCurrent)(HDC,HGLRC);
    typedef BOOL (WINAPI * PFN_wglShareLists)(HGLRC,HGLRC);

    extern PFN_wglGetProcAddress wglGetProcAddress;
	extern PFN_wglMakeCurrent wglMakeCurrent;
	extern PFN_wglCreateContext wglCreateContext;
	extern PFN_wglDeleteContext wglDeleteContext;
    extern PFN_wglGetCurrentDC wglGetCurrentDC;
    extern PFN_wglGetCurrentContext wglGetCurrentContext;
    extern PFN_wglShareLists wglShareLists;
}

#endif // __WGL_CONTEXT_H__
