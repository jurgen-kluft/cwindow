#pragma once

#include "cwindow/state.h"

#if defined(CWINDOW_WIN32)
#define MainArgs HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow
#define MainArgsVars hInstance, hPrevInstance, lpCmdLine, nCmdShow
#elif defined(CWINDOW_DARWIN) || defined(CWINDOW_UIKIT)
#define MainArgs int argc, const char *argv[], void *application
#define MainArgsVars argc, argv, application
#elif defined(CWINDOW_XCB)
#define MainArgs int argc, const char *argv[], xcb_connection_t *connection, xcb_screen_t *screen
#define MainArgsVars argc, argv, connection, screen
#elif defined(CWINDOW_XLIB) || defined(CWINDOW_MIR) || defined(CWINDOW_WAYLAND) || defined(CWINDOW_WASM) || defined(CWINDOW_NOOP)
#define MainArgs int argc, const char *argv[]
#define MainArgsVars argc, argv
#endif

namespace cwin
{
    bool init(MainArgs);
    const XWinState& getXWinState();
} // namespace cwin
