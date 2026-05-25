#ifndef __CWINDOW_WINDOW_LINUX_H__
#define __CWINDOW_WINDOW_LINUX_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "cwindow/c_types.h"

namespace nwindow
{
    struct WindowStateLinux
    {
        void*       hInstance;
        void*       hPrevInstance;
        const char* lpCmdLine;
        s32         nCmdShow;

        WindowStateLinux()
            : hInstance(nullptr)
            , hPrevInstance(nullptr)
            , lpCmdLine(nullptr)
            , nCmdShow(0)
        {
        }

        WindowStateLinux(void* hInstance, void* hPrevInstance, const char* lpCmdLine, s32 nCmdShow)
            : hInstance(hInstance)
            , hPrevInstance(hPrevInstance)
            , lpCmdLine(lpCmdLine)
            , nCmdShow(nCmdShow)
        {
        }
    };
}  // namespace nwindow

#endif
