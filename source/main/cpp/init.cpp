#include "cwindow/init.h"
#include "cwindow/state.h"

namespace nwindow
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

    const nwindow::XWinState& getXWinState() { return xWinState; }
    
} // namespace nwindow
