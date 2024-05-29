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
    };


} // namespace nwindow