#ifndef __CWINDOW_WINDOW_WIN32_H__
#define __CWINDOW_WINDOW_WIN32_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "cwindow/c_types.h"
#include "cwindow/c_windowdesc.h"

namespace nwindow
{
    class WindowPC
    {
    public:
        HINSTANCE hInstance;
        HINSTANCE hPrevInstance;
        LPSTR     lpCmdLine;
        int       nCmdShow;

        WindowPC()
            : hInstance(nullptr)
            , hPrevInstance(nullptr)
            , lpCmdLine(nullptr)
            , nCmdShow(0)
        {
        }

        WindowPC(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
            : hInstance(hInstance)
            , hPrevInstance(hPrevInstance)
            , lpCmdLine(lpCmdLine)
            , nCmdShow(nCmdShow)
        {
        }
    };


}  // namespace nwindow

#endif
