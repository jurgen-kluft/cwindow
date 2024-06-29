#ifndef __CWINDOW_WINDOW_MAC_H__
#define __CWINDOW_WINDOW_MAC_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "cwindow/private/c_eventqueue.h"

namespace nwindow
{
    class WindowMac
    {
    public:
        int          argc;
        const char** argv;
        void*        application;
        void*        window;  // XWinWindow
        void*        view;    // XWinView

        WindowMac()
            : argc(0)
            , argv(nullptr)
            , application(nullptr)
            , window(nullptr)
            , view(nullptr)
        {
        }

        WindowMac(int argc, const char** argv, void* application)
            : argc(argc)
            , argv(argv)
            , application(application)
            , window(nullptr)
            , view(nullptr)
        {
        }
    };

}  // namespace nwindow

#endif  // __CWINDOW_WINDOW_MAC_H__
