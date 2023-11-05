import sys
import glob
import os
import shutil

platform = sys.platform
app_name = "wastrel"
src_dir = "src"
cmp_flag = "-g"
build_mode = "DEBUG_BUILD"
console_output_on_windows = True

if platform == "darwin":
	libs = [
		"SDL2",
		"SDL2main",
		"SDL2_mixer",
		"SDL2_image"
	]

	frameworks = [
	]

	defines = [
	]

	env = Environment (
		ENV = {
			"PATH" : os.environ["PATH"]
		}
	)

	src_paths = [
		"build/" + src_dir + "/*.c",
		"build/" + src_dir + "/utils/*.c",
		"build/" + src_dir + "/core/*.c",
		"build/" + src_dir + "/game/*.c",
		"build/" + src_dir + "/visuals/*.c"
	]

	env.VariantDir("build", ".", 0)

	sources = []

	for path in src_paths:
		sources += Glob(path)

	env.Program (
		tools = ["default", "Xcode"],
		target = "bin/" + app_name,
		source = sources,
		LIBS = libs,
		LIBPATH = [
			"/opt/homebrew/lib"
		],
		CPPPATH = [
			"/opt/homebrew/include",
			"/opt/homebrew/include/lua",
			"/opt/homebrew/opt/sqlite/include",
			src_dir
		],
		CPPDEFINES = defines,
		CFLAGS = [cmp_flag, "-std=c17"],
		CXXFLAGS = [cmp_flag, "-std=c++17"],
		FRAMEWORKPATH = [
		],
		FRAMEWORKS = frameworks
	)


if platform == "win32":
	libs = [
		"mingw32",
		"SDL2main",
		"SDL2",
		"SDL2_mixer",
		"SDL2_image"
	]

	defines = [
		build_mode
	]

	env = Environment (
		tools = ["mingw"],
		ENV = {
			"PATH" : os.environ["PATH"],
			"TMP" : os.environ["TMP"],
			"TEMP" : os.environ["TEMP"],
			"SystemDrive" : os.environ["SystemDrive"],
			"SystemRoot" : os.environ["SystemRoot"],
		}
	)

	if console_output_on_windows == False:
		env.Append(LINKFLAGS = ['-Wl, -subsystem,windows'])

	src_paths = [
		"build/" + src_dir + "/*.c",
		"build/" + src_dir + "/utils/*.c",
		"build/" + src_dir + "/core/*.c",
		"build/" + src_dir + "/game/*.c",
		"build/" + src_dir + "/visuals/*.c"
	]

	env.VariantDir("build", ".", 0)

	sources = []

	for path in src_paths:
		sources += Glob(path)


	env.Program (
		target = "bin/" + app_name,
		source = sources,
		LIBS = libs,
		LIBPATH = [
			"C:\\msys64\\mingw64\\lib"
		],
		CPPPATH = [
			src_dir
		],
		CPPDEFINES = defines,
		CFLAGS = [cmp_flag, "-std=c17"],
		CXXFLAGS = [cmp_flag, "-std=c++20"]
	)
