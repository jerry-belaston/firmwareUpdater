# Quick user build & installation

Prerequisite: 
- The project uses C++20 features, therefore make sure the version of your chosen compiler, no matter the platform, support it.
- The project currently requires at least the version 5.15.2 of Qt.

## Windows with Visual Studio

- Install [Git](https://git-scm.com/) via the Windows installer (note that we will use Git bash tool coming with Git to run bash scripts).
- Install [CMake](https://cmake.org/download/) using the CMake installer and make sure to select the option to add CMake to the system `PATH` during its installation.
- Install [Microsoft Visual Studio](https://visualstudio.microsoft.com/downloads/) and make sure the `Desktop development with C++` is included (it should enable MSVC build tools and Windows 10 SDK).
- Install [Qt](https://doc.qt.io/qt-5/gettingstarted.html) using the online installer (which requires signing into a Qt Account). Make sure to select during the installation the following components:
    - MSVC compiler version (32 and/or 64bit)
    - OpenSSL Toolkit (32 or/and 64bit)
    - Qt Debugging Tools
    - Qt Creator
    - Qt Creator CDB Debugger Support
- Add `Qt5_DIR` variable to your user environment variables. The value must be the path of your Qt MSVC compiler directory (for instance `C:\Qt\5.15.2\msvc2019`).
- Clone this repository.
- Run `run_cmake.sh` script which will generate `_build_<toolset>` solution folder.
- Open `.sln` generated solution with Visual Studio.
- Select `Debug` or `Release` configuration.
- Build and run the `firmwareUpdater` default target: you are done.

## Windows with Qt Creator

- Install [Qt](https://doc.qt.io/qt-5/gettingstarted.html) using the online installer (which requires signing into a Qt Account). Make sure to select during the installation the following components:
    - MSVC compiler version 32bit and/or 64bit
    - OpenSSL Toolkit (32 or/and 64bit)
    - Qt Debugging Tools
    - Qt Creator
    - Qt Creator CDB Debugger Support
- Clone this repository.
- Open `firmwareUpdater.pro` project with Qt Creator.
- In the "Configure Project", select MSVC compiler kit and click "configure".
- Select `Debug` or `Release` configuration.
- Build and Run project. You are done!
