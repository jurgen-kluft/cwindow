#pragma once

struct winstate_t
{
    int          argc;
    const char** argv;
    void*        application;

    winstate_t()
        : argc(0)
        , argv(nullptr)
        , application(nullptr)
    {
    }
	
    winstate_t(int argc, const char** argv, void* application)
        : argc(argc)
        , argv(argv)
        , application(application)
    {
    }
};
