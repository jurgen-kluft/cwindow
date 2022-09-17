#pragma once

struct winstate_t
{
	int          argc;
	const char** argv;
	void*        application;

	winstate_t(int argc, const char** argv, void* application)
		: argc(argc)
		, argv(argv)
		, application(application)
	{
	}
};

const winstate_t& getWinState();
