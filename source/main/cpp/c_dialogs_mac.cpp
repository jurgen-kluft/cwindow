#include "cwindow/c_dialogs.h"

#include <CoreFoundation/CoreFoundation.h>

namespace nwindow
{
#define IDOK     1
#define IDCANCEL 2
    int showMessageBox(const MessageDesc& mdesc)
    {
        // convert the strings from char* to CFStringRef
        CFStringRef header_ref  = CFStringCreateWithCString(NULL, mdesc.title, strlen(mdesc.title));
        CFStringRef message_ref = CFStringCreateWithCString(NULL, mdesc.text, strlen(mdesc.text));

        CFOptionFlags result;  // result code from the message box

        // launch the message box
        CFUserNotificationDisplayAlert(0,                                  // no timeout
                                       kCFUserNotificationNoteAlertLevel,  // change it depending message_type flags ( MB_ICONASTERISK.... etc.)
                                       NULL,                               // icon url, use default, you can change it depending message_type flags
                                       NULL,                               // not used
                                       NULL,                               // localization of strings
                                       header_ref,                         // header text
                                       message_ref,                        // message text
                                       NULL,                               // default "ok" text in button
                                       CFSTR("Cancel"),                    // alternate button title
                                       NULL,                               // other button title, null--> no other button
                                       &result                             // response flags
        );

        // Clean up the strings
        CFRelease(header_ref);
        CFRelease(message_ref);

        // Convert the result
        if (result == kCFUserNotificationDefaultResponse)
            return IDOK;
        else
            return IDCANCEL;
    }

}  // namespace nwindow
