#ifndef __CWINDOW_WINDOW_DIALOG_H__
#define __CWINDOW_WINDOW_DIALOG_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace nwindow
{
    enum class MessageType
    {
        Warning,
        MessageTypeMax
    };

    struct MessageDesc
    {
        MessageType type;  // OK, Error, etc.
        const char* text;
        const char* title;
    };

    int showMessageBox(const MessageDesc& mdesc);

}  // namespace nwindow

#endif
