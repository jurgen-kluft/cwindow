#include "cwindow/init.h"
#include "cwindow/state.h"

namespace cwin
{
    namespace
    {
        XWinState xWinState;
    }

    bool init(MainArgs)
    {
        xWinState = XWinState(MainArgsVars);
        return true;
    }
    const cwin::XWinState& getXWinState() { return xWinState; }
} // namespace cwin
