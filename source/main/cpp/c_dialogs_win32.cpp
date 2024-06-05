#include "cwindow/private/c_dialogs_win32.h"
#include "cwindow/c_dialogs.h"

namespace nwindow
{
    // https://docs.microsoft.com/en-us/windows/desktop/learnwin32/example--the-open-dialog-box

    int showMessageBox(const MessageDesc& mdesc)
    {
        UINT flags = MB_APPLMODAL | MB_SETFOREGROUND | MB_ICONINFORMATION;
        flags |= MB_OK;
        return MessageBoxA(nullptr, LPCSTR(mdesc.text), LPCSTR(mdesc.title), flags);
    }

} // namespace nwindow
