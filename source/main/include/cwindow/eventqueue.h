#pragma once

#include "cwindow/event.h"

#ifdef CWINDOW_WIN32
#include "cwindow/eventqueue_win32.h"
#elif CWINDOW_DARWIN
#include "cwindow/eventqueue_mac.h"
#elif CWINDOW_XLIB
#include "cwindow/eventqueue_linux.h"
#else

#endif