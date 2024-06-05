#ifndef __CWINDOW_WINDOW_DESC_H__
#define __CWINDOW_WINDOW_DESC_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "cwindow/c_types.h"

namespace nwindow
{
    struct WindowDesc
    {
        WindowDesc();

        // Transform

        s32 x;         // Window X position based on Top Left
        s32 y;         // Window Y position based on Top Left
        u32 width;     // Window Width
        u32 height;    // Window Height
        u32 minWidth;  // Window minimum width
        u32 minHeight; // Window minimum height
        u32 maxWidth;  // Window maximum width
        u32 maxHeight; // Window maximum height

        // Window Settings

        bool centered;      // Should this window be centered on creation?
        bool resizable;     // Whether this window is resizable
        bool movable;       // Whether this window is movable
        bool closable;      // Whether this window is closable
        bool minimizable;   // Whether this window is minimizable
        bool maximizable;   // whether this window is maximizable
        bool canFullscreen; // Whether this window can be fullscreened

        // Color and Style

        u32  backgroundColor; // Hexidecimal background color (0xffffffff = white)
        bool transparent;     // makes this window transparent
        bool frame;           // does this window feature a frame?
        bool hasShadow;       // if this window has a shadow

        // States

        bool visible;    // Is this window visible?
        bool fullscreen; // Is this window full screen?
        bool modal;      // Is this window a modal?

        // App Data

        const char* title;    // Window Title
        const char* name;     // Window name ID
        const char* iconPath; // Icon Path
    };

} // namespace nwindow

#endif // __CWINDOW_WINDOW_DESC_H__
