#!/bin/bash

# Visual Studio solution system builder

# Get arguments
while [ $# -gt 0 ]
do
	case "$1" in
		-fc)
			#Force cleanup of repository
			git clean -dxf .
			;;
		-h)
			#Display usage
			echo "Usage: $0 [-fc] [-h]"
			exit 0
			;;
		*)
			;;
	esac
	shift
done

#Run Cmake
qt_version="5.15.2"
qt_version_major="${qt_version%%.*}"
qt_root_dir="C:/Qt"
qt_toolchain_dir="${qt_root_dir}/${qt_version}/msvc2019"
build_system_generator="Visual Studio 16 2019"
build_system_platform="Win32"
build_dir="_build_win_msvc_${build_system_platform}"
source_dir=.
cmake -S "$source_dir" -B "$build_dir" -G "$build_system_generator" -A "$build_system_platform"  -DCMAKE_PREFIX_PATH="${qt_toolchain_dir}" -DQT_ROOT_DIR="${qt_root_dir}" -DQT_PACKAGE_VERSION_MAJOR="${qt_version_major}"

# Wait user action to exit
echo
read -p "Press any key to exit..."