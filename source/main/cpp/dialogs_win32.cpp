#include "cwindow/dialogs_win32.h"
#include "cwindow/dialogs.h"

namespace cwin
{
    // https://docs.microsoft.com/en-us/windows/desktop/learnwin32/example--the-open-dialog-box

    void showMessageBox(const MessageDesc& mdesc)
    {
        UINT flags = MB_APPLMODAL | MB_SETFOREGROUND | MB_ICONINFORMATION;
        flags |= MB_OK;
        MessageBox(nullptr, LPCWSTR("Text"), LPCWSTR("Title"), flags);
    }

} // namespace cwin