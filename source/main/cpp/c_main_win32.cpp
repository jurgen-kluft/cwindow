#include "cwindow/c_main.h"

#include <Windows.h>
#include <stdio.h>

#include "cwindow/private/c_window_win32.h"

nwindow::WindowPC g_WindowPC;

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
    AllocConsole();
    FILE* pCout = nullptr;
    FILE* pCerr = nullptr;
    freopen_s(&pCout, "CONOUT$", "w+", stdout);
    freopen_s(&pCerr, "CONOUT$", "w+", stderr);
#endif
    // Setup command line arguments.
    int    argc;
    char** argv;

    // Use the CommandLine functions to get the command line arguments.
    // Unfortunately, Microsoft outputs
    // this information as wide characters for Unicode, and we simply want the
    // Ascii version to be compatible
    // with the non-Windows side.  So, we have to convert the information to
    // Ascii character strings.
    LPWSTR* commandLineArgs = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (NULL == commandLineArgs)
    {
        argc = 0;
    }

    if (argc > 0)
    {
        argv = (char**)malloc(sizeof(char*) * argc);
        if (argv == NULL)
        {
            argc = 0;
        }
        else
        {
            for (int iii = 0; iii < argc; iii++)
            {
                size_t wideCharLen  = wcslen(commandLineArgs[iii]);
                size_t numConverted = 0;

                argv[iii] = (char*)malloc(sizeof(char) * (wideCharLen + 1));
                if (argv[iii] != NULL)
                {
                    wcstombs_s(&numConverted, argv[iii], wideCharLen + 1, commandLineArgs[iii], wideCharLen + 1);
                }
            }
        }
    }
    else
    {
        argv = NULL;
    }

    g_WindowPC = nwindow::WindowPC(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    cwindow_main(argc, (const char**)argv);

    // Free up the items we had to allocate for the command line arguments.
    if (argc > 0 && argv != NULL)
    {
        for (int iii = 0; iii < argc; iii++)
        {
            if (argv[iii] != NULL)
            {
                free(argv[iii]);
            }
        }
        free(argv);
    }

#ifdef _DEBUG
    FreeConsole();
    if (pCout != nullptr) fclose(pCout);
    if (pCerr != nullptr) fclose(pCerr);
#endif

    return 0;
}
