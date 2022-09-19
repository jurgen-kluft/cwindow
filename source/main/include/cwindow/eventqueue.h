#pragma once

#include "cwindow/event.h"

#ifdef CWINDOW_WIN32
#include "cwindow/private/eventqueue_win32.h"
#elif CWINDOW_DARWIN
#include "cwindow/private/eventqueue_mac.h"
#elif CWINDOW_LINUX
#include "cwindow/private/eventqueue_linux.h"
#else

#endif