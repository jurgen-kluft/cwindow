#pragma once

#ifdef CWINDOW_WIN32
#include "cwindow/private/window_win32.h"
#elif CWINDOW_DARWIN
#include "cwindow/private/window_mac.h"
#elif CWINDOW_LINUX
#include "cwindow/private/window_linux.h"
#else
#pragma error "Unsupported platform"
#endif

#include "cwindow/c_windowdesc.h"

namespace cwin = nwindow;
