local native = require('tundra.native')

-----------------------------------------------------------------------------------------------------------------------

local mac_opts = {
    "-I.",
    "-Wno-new-returns-null",
    "-Wno-missing-braces",
    "-Wno-c++11-compat-deprecated-writable-strings",
    "-Wno-null-dereference",
    "-Wno-format",
    "-fno-strict-aliasing",
    "-fno-omit-frame-pointer",
	"-Wno-write-strings",
    "-Wno-array-bounds",
    "-Wno-attributes",
    "-Wno-unused-value",
    "-Wno-unused-function",
    "-Wno-unused-variable",
    "-Wno-unused-result",
    { "-O2", "-g"; Config = "*-*-test" },
    { "-O0", "-g"; Config = "*-*-debug" },
    { "-O3", "-g"; Config = "*-*-release" },
}

-----------------------------------------------------------------------------------------------------------------------

local macosx = {
    Env = {
        CCOPTS =  {
            mac_opts,
        },

        CXXOPTS = {
            mac_opts,
            "-std=c++14",
			"-arch x86_64",
        },

        SHLIBOPTS = {
			"-lstdc++",
			{ "-fsanitize=address"; Config = "*-*-debug-asan" },
		},

        PROGCOM = {
			"-lstdc++",
			{ "-fsanitize=address"; Config = "*-*-debug-asan" },
		},
    },

	ReplaceEnv = {
		OBJECTROOT = "target",
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

-----------------------------------------------------------------------------------------------------------------------

local linux_opts = {
    "-I.",
    "-Wno-new-returns-null",
    "-Wno-missing-braces",
    "-Wno-c++11-compat-deprecated-writable-strings",
    "-Wno-null-dereference",
    "-Wno-format",
    "-fno-strict-aliasing",
    "-fno-omit-frame-pointer",
	"-Wno-write-strings",
    "-Wno-array-bounds",
    "-Wno-attributes",
    "-Wno-unused-value",
    "-Wno-unused-function",
    "-Wno-unused-variable",
    "-Wno-unused-result",
    "-DOBJECT_DIR=\\\"$(OBJECTDIR)\\\"",
    "-I$(OBJECTDIR)",
    "-Wall",
    "-fPIC",
    "-msse2",   -- TODO: Separate gcc options for x64/arm somehow?
    { "-O2", "-g"; Config = "*-*-test" },
    { "-O0", "-g"; Config = "*-*-debug" },
    { "-O3", Config = "*-*-release" },
}

local linux = {
    Env = {

        CCOPTS = {
			"-Werror=incompatible-pointer-types",
            linux_opts,
        },

        CXXOPTS = {
            linux_opts,
        },
    },

    ReplaceEnv = {
        LD = "c++",
		OBJECTROOT = "target",
	},
}

-----------------------------------------------------------------------------------------------------------------------

local win64_opts = {
    "/EHsc", "/FS", "/MD", "/W3", "/I.", "/DUNICODE", "/D_UNICODE", "/DWIN32", "/D_CRT_SECURE_NO_WARNINGS",
    "\"/DOBJECT_DIR=$(OBJECTDIR:#)\"",
    { "/Od"; Config = "*-*-debug" },
    { "/O2"; Config = "*-*-release" },
}

local win64 = {
    Env = {

        GENERATE_PDB = "1",
        CCOPTS = {
            win64_opts,
        },

        CXXOPTS = {
            win64_opts,
        },

        OBJCCOM = "meh",
    },
    ReplaceEnv = {
        OBJECTROOT = "target",
    },
}

-----------------------------------------------------------------------------------------------------------------------

Build {
    Passes = {
        BuildTools = { Name = "BuildTools", BuildOrder = 1 },
        GenerateSources = { Name = "GenerateSources", BuildOrder = 2 },
    },

    Units = {
        "units.lua",
    },

    Configs = {
        Config { Name = "macos-clang", DefaultOnHost = "macosx", Inherit = macosx, Tools = { "clang-osx" } },
        Config { Name = "win64-msvc", DefaultOnHost = { "windows" }, Inherit = win64, Tools = { "msvc-vs2019" } },
        Config { Name = "linux-gcc", DefaultOnHost = { "linux" }, Inherit = linux, Tools = { "gcc" } },
        Config { Name = "linux-clang", DefaultOnHost = { "linux" }, Inherit = linux, Tools = { "clang" } },
    },

    IdeGenerationHints = {
        Msvc = {
            -- Remap config names to MSVC platform names (affects things like header scanning & debugging)
            PlatformMappings = {
                ['win64-msvc'] = 'x64',
            },

            -- Remap variant names to MSVC friendly names
            VariantMappings = {
                ['release']    = 'Release',
                ['debug']      = 'Debug',
            },
        },

        MsvcSolutions = {
            ['libglfw.sln'] = { }
        },

    },

    -- Variants = { "debug", "test", "release" },
    Variants = { "debug", "release" },
    SubVariants = { "default", "test" },
}
