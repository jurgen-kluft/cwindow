#pragma once

namespace nwindow
{
    enum class MessageType
    {
        Warning,
        MessageTypeMax
    };

    struct MessageDesc
    {
        MessageType type; // OK, Error, etc.
        const char* text;
        const char* title;
    };


} // namespace nwindow
