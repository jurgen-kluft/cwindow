#pragma once

#ifdef CWINDOW_WIN32
#include "cwindow/window_win32.h"
#elif CWINDOW_DARWIN
#include "cwindow/window_darwin.h"
#elif CWINDOW_LINUX
#include "cwindow/window_linux.h"
#else

#endif
