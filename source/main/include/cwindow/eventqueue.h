#pragma once

#include "cwindow/event.h"

#ifdef CWINDOW_WIN32
#include "cwindow/eventqueue_win32.h"
#elif CWINDOW_COCOA
#include "cwindow/eventqueue_darwin.h"
#elif CWINDOW_XLIB
#include "cwindow/eventqueue_linux.h"
#else

#endif