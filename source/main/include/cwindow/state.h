#pragma once

#if defined(CWINDOW_WIN32)
#include <Windows.h>
#elif defined(CWINDOW_XCB)
#include <xcb/xcb.h>
#elif defined(CWINDOW_XLIB)
#include <X11/Xlib.h>
#endif

namespace nwindow
{
    struct XWinState
    {

#if defined(CWINDOW_WIN32)

        HINSTANCE hInstance;
        HINSTANCE hPrevInstance;
        LPSTR     lpCmdLine;
        int       nCmdShow;

        XWinState(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
            : hInstance(hInstance)
            , hPrevInstance(hPrevInstance)
            , lpCmdLine(lpCmdLine)
            , nCmdShow(nCmdShow)
        {
        }
#elif defined(CWINDOW_DARWIN) || defined(CWINDOW_UIKIT)

        int          argc;
        const char** argv;
        void*        application;

        XWinState(int argc, const char** argv, void* application)
            : argc(argc)
            , argv(argv)
            , application(application)
        {
        }

#else

        int          argc;
        const char** argv;

        XWinState(int argc, const char** argv)
            : argc(argc)
            , argv(argv)
        {
        }

#endif
        XWinState() {}
    };
} // namespace nwindow
