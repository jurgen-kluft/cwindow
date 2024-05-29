#pragma once

struct winstate_t
{
    HINSTANCE   hInstance;
    HINSTANCE   hPrevInstance;
    LPSTR       lpCmdLine;
    int         nCmdShow;

    winstate_t()
        : hInstance(nullptr)
        , hPrevInstance(nullptr)
        , lpCmdLine(nullptr)
        , nCmdShow(0)
    {
    }

    winstate_t(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
        : hInstance(hInstance)
        , hPrevInstance(hPrevInstance)
        , lpCmdLine(lpCmdLine)
        , nCmdShow(nCmdShow)
    {
    }
};
