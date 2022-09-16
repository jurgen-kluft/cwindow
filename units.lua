require "tundra.syntax.glob"
require "tundra.path"
require "tundra.util"

local cbase_library = StaticLibrary {
	Name = "cbase",
	Env = {
		CPPPATH = {
			"../cbase/source/main/cpp",
			"../cbase/source/main/include",
		},
		CPPDEFS = {
			{
				"TARGET_DEBUG",
				"TARGET_DEV",
				"_DEBUG",
				"TARGET_MAC",
				Config = "*-*-debug"
			},
			{
				"TARGET_RELEASE",
				"TARGET_DEV",
				"NDEBUG",
				"TARGET_MAC",
				Config = "*-*-release"
			},
			{ "TARGET_MAC", Config = "macos-*-*" },
			{ "TARGET_TEST", Config = "*-*-*-test" },
		},
	},
	Includes = {
		"../cbase/source/main/include",
	},
	Sources = {
		                "../cbase/source/main/cpp/c_allocator.cpp",
                "../cbase/source/main/cpp/c_allocator_system_mac.cpp",
                "../cbase/source/main/cpp/c_base.cpp",
                "../cbase/source/main/cpp/c_binary_search.cpp",
                "../cbase/source/main/cpp/c_btree.cpp",
                "../cbase/source/main/cpp/c_buffer.cpp",
                "../cbase/source/main/cpp/c_console.cpp",
                "../cbase/source/main/cpp/c_console_mac.cpp",
                "../cbase/source/main/cpp/c_context.cpp",
                "../cbase/source/main/cpp/c_darray.cpp",
                "../cbase/source/main/cpp/c_debug.cpp",
                "../cbase/source/main/cpp/c_guid.cpp",
                "../cbase/source/main/cpp/c_hash.cpp",
                "../cbase/source/main/cpp/c_hash2.cpp",
                "../cbase/source/main/cpp/c_hbb.cpp",
                "../cbase/source/main/cpp/c_int128.cpp",
                "../cbase/source/main/cpp/c_int256.cpp",
                "../cbase/source/main/cpp/c_int64.cpp",
                "../cbase/source/main/cpp/c_log.cpp",
                "../cbase/source/main/cpp/c_log_to_console.cpp",
                "../cbase/source/main/cpp/c_map.cpp",
                "../cbase/source/main/cpp/c_memory.cpp",
                "../cbase/source/main/cpp/c_printf.cpp",
                "../cbase/source/main/cpp/c_qsort.cpp",
                "../cbase/source/main/cpp/c_runes.cpp",
                "../cbase/source/main/cpp/c_slice.cpp",
                "../cbase/source/main/cpp/c_sscanf.cpp",
                "../cbase/source/main/cpp/c_tree.cpp",
                "../cbase/source/main/cpp/c_uint128.cpp",
                "../cbase/source/main/cpp/c_uint256.cpp",
                "../cbase/source/main/cpp/c_uint64.cpp",
                "../cbase/source/main/cpp/c_va_list.cpp",

	},
}


local cunittest_library = StaticLibrary {
	Name = "cunittest",
	Env = {
		CPPPATH = {
			"../cunittest/source/main/cpp",
			"../cunittest/source/main/include",
		},
		CPPDEFS = {
			{
				"TARGET_DEBUG",
				"TARGET_DEV",
				"_DEBUG",
				"TARGET_MAC",
				Config = "*-*-debug"
			},
			{
				"TARGET_RELEASE",
				"TARGET_DEV",
				"NDEBUG",
				"TARGET_MAC",
				Config = "*-*-release"
			},
			{ "TARGET_MAC", Config = "macos-*-*" },
			{ "TARGET_TEST", Config = "*-*-*-test" },
		},
	},
	Includes = {
		"../cunittest/source/main/include",
	},
	Sources = {
		                "../cunittest/source/main/cpp/entry/ut_Entry_Mac.cpp",
                "../cunittest/source/main/cpp/ut_AssertException.cpp",
                "../cunittest/source/main/cpp/ut_Checks.cpp",
                "../cunittest/source/main/cpp/ut_ReportAssert.cpp",
                "../cunittest/source/main/cpp/ut_Stdout_Mac.cpp",
                "../cunittest/source/main/cpp/ut_Stdout_Win32.cpp",
                "../cunittest/source/main/cpp/ut_StringBuilder.cpp",
                "../cunittest/source/main/cpp/ut_Test.cpp",
                "../cunittest/source/main/cpp/ut_TestList.cpp",
                "../cunittest/source/main/cpp/ut_TestReporter.cpp",
                "../cunittest/source/main/cpp/ut_TestReporterStdout.cpp",
                "../cunittest/source/main/cpp/ut_TestReporterTeamCity.cpp",
                "../cunittest/source/main/cpp/ut_TestResults.cpp",
                "../cunittest/source/main/cpp/ut_TestRunner.cpp",
                "../cunittest/source/main/cpp/ut_TestState.cpp",
                "../cunittest/source/main/cpp/ut_Test_Mac.cpp",
                "../cunittest/source/main/cpp/ut_Test_Win32.cpp",
                "../cunittest/source/main/cpp/ut_TimeConstraint.cpp",
                "../cunittest/source/main/cpp/ut_TimeHelpers_Mac.cpp",
                "../cunittest/source/main/cpp/ut_TimeHelpers_Win32.cpp",
                "../cunittest/source/main/cpp/ut_Utils.cpp",
                "../cunittest/source/main/cpp/ut_Utils_Mac.cpp",
                "../cunittest/source/main/cpp/ut_Utils_Win32.cpp",

	},
}


local cwindow_library = StaticLibrary {
	Name = "cwindow",
	Env = {
		CPPPATH = {
			"source/main/cpp",
			"source/main/include",
			"../cbase/source/main/include",
			"../cbase/source/main/cpp",
		},
		CPPDEFS = {
			{
				"TARGET_DEBUG",
				"TARGET_DEV",
				"_DEBUG",
				"TARGET_MAC",
				"CWINDOW_DARWIN",
				Config = "*-*-debug"
			},
			{
				"TARGET_RELEASE",
				"TARGET_DEV",
				"NDEBUG",
				"TARGET_MAC",
				"CWINDOW_DARWIN",
				Config = "*-*-release"
			},
			{ "TARGET_MAC", Config = "macos-*-*" },
			{ "TARGET_TEST", Config = "*-*-*-test" },
		},
	},
	Includes = {
		"source/main/include",
		"../cbase/source/main/include",
	},
	Sources = {
		                "source/main/cpp/dialogs.cpp",
                "source/main/cpp/event.cpp",
                "source/main/cpp/init.cpp",
                "source/main/cpp/eventqueue_mac.mm",
                "source/main/cpp/main_mac.mm",
                "source/main/cpp/window_mac.mm",

	},
}

local unittest = Program {
	Name = "cwindow_test",
	Env = {
		CPPPATH = {
			"../cbase/source/main/include",
			"../cbase/source/main/cpp",
			"../cunittest/source/main/include",
			"../cunittest/source/main/cpp",
			"source/main/include",
			"source/main/cpp",
		},
		CPPDEFS = {
			{
				"TARGET_RELEASE",
				"TARGET_DEV",
				"NDEBUG",
				"TARGET_MAC",
				"CWINDOW_DARWIN",
				Config = "*-*-release"
			},
			{
				"TARGET_DEBUG",
				"TARGET_DEV",
				"_DEBUG",
				"TARGET_MAC",
				"CWINDOW_DARWIN",
				Config = "*-*-debug"
			},
			{ "TARGET_MAC", Config = "macos-*-*" },
			{ "TARGET_TEST", Config = "*-*-*-test" },
		},
	},
	Sources = {
		                "source/test/cpp/test_main.cpp",
                "source/main/cpp/eventqueue_mac.mm",
                "source/main/cpp/main_mac.mm",
                "source/main/cpp/window_mac.mm",

	},
	Includes = {
		"source/main/include","source/test/include","../cbase/source/main/include","../cunittest/source/main/include"
	},
	Depends = {
		cbase_library,cunittest_library,cwindow_library
	},
	Frameworks = {
		{ "Cocoa" },
		{ "Metal" },
		{ "OpenGL" },
		{ "IOKit" },
		{ "CoreVideo" },
		{ "QuartzCore" },
	},
}
Default(unittest)
