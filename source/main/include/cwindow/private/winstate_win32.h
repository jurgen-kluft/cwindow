#pragma once

struct winstate_t
{
    void*       hInstance;
    void*       hPrevInstance;
    const char* lpCmdLine;
    int         nCmdShow;

    winstate_t(void* hInstance, void* hPrevInstance, const char* lpCmdLine, int nCmdShow)
        : hInstance(hInstance)
        , hPrevInstance(hPrevInstance)
        , lpCmdLine(lpCmdLine)
        , nCmdShow(nCmdShow)
    {
    }
};

const winstate_t& getWinState();
