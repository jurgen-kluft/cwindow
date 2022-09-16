#pragma once

namespace cwin
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


} // namespace cwin