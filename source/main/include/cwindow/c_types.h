#ifndef __CWINDOW_TYPES_H__
#define __CWINDOW_TYPES_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

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

    struct UVec2
    {
        s32 x;
        s32 y;

        UVec2() : x(0), y(0) {}
        UVec2(s32 x, s32 y) : x(x), y(y) {}

        UVec2& operator+=(const UVec2& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }

        UVec2& operator-=(const UVec2& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }

        UVec2 operator+(const UVec2& rhs) const
        {
            return { x + rhs.x, y + rhs.y };
        }

        UVec2 operator-(const UVec2& rhs) const
        {
            return { x - rhs.x, y - rhs.y };
        }

        bool operator==(const UVec2& rhs) const
        {
            return x == rhs.x && y == rhs.y;
        }

        bool operator!=(const UVec2& rhs) const
        {
            return x != rhs.x || y != rhs.y;
        }
    };

    struct UBox2
    {
    public:
        UVec2 min;
        UVec2 max;

        UVec2 size() const
        {
            return { max.x - min.x, max.y - min.y };
        }

        bool operator==(const UBox2& rhs) const
        {
            return min == rhs.min && max == rhs.max;
        }

        bool operator!=(const UBox2& rhs) const
        {
            return min != rhs.min || max != rhs.max;
        }
    };
}  // namespace nwindow

#endif
