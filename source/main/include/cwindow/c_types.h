#include "cbase/c_target.h"

namespace nwindow
{
    typedef ncore::u64 u64;
    typedef ncore::s64 s64;
    typedef ncore::s32 s32;
    typedef ncore::u32 u32;
    typedef ncore::s16 s16;
    typedef ncore::u16 u16;
    typedef ncore::s8  s8;
    typedef ncore::u8  u8;

#if defined(CWINDOW_WIN32)
    typedef u64   hinstance_t;
    typedef void* hwnd_t;
    typedef u64   lparam_t;
    typedef u64   wparam_t;
    typedef void* msg_t;
    typedef s32   lresult_t;
#endif
} // namespace nwindow