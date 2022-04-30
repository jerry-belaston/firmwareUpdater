cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)

# Project base information
project(firmwareUpdater VERSION 2.0.0.0 LANGUAGES CXX)

# Project properties
set(FU_VERSIONMAJ		${PROJECT_VERSION_MAJOR})
set(FU_VERSIONMIN		${PROJECT_VERSION_MINOR})
set(FU_VERSIONSUB		${PROJECT_VERSION_PATCH})
set(FU_VERSIONREV		${PROJECT_VERSION_TWEAK})
set(FU_PRODUCTNAME		"Firmware Updater")
set(FU_PRODUCTVERSION	"${FU_VERSIONMAJ}.${FU_VERSIONMIN}.${FU_VERSIONSUB}.${FU_VERSIONREV}")
set(FU_FILEDESCRIPTION	"${FU_PRODUCTNAME}")
set(FU_FILEVERSION		"${FU_PRODUCTVERSION}")
set(FU_COMPANYNAME		"HOME")
set(FU_COMMENTS			"Firmware Updater application")
set(FU_LEGALTRADEMARKS	"Firmware Updater is a trademark of HOME")
set(FU_LEGALCOPYRIGHT	"Copyright (c) HOME, Jerry Belaston. All rights reserved.")
set(FU_BUNDLEIDENTIFIER	"com.home.firmwareUpdater")
set(FU_URLABOUTINFO		"http://www.github.com/firmwareUpdater")

# Add common funtions folder
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/ext/cmake)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set project flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include Qt common functions
include(commonQt)

# Set warnings level max (and warnings as errors)
set_warnings_level_max()

# Setup Qt project
set(QT_MODULES "Core;Gui;Network;Widgets")
commonQt_setup_qt(Qt${QT_VERSION_MAJOR} "${QT_MODULES}")

# Add project source files
set(SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/src/core/common/type.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/core/controller/controller.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/core/controller/controller.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/core/controller/controllerMulti.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/core/controller/controllerMulti.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/core/controller/controllerSingle.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/core/controller/controllerSingle.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/core/controller/viewInterface.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/core/model/device/private/defaultDevices.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/core/model/device/private/deviceSerialization.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/core/model/device/private/deviceSerialization.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/core/model/device/discovery.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/core/model/device/discovery.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/core/model/device/uploadManager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/core/model/device/uploadManager.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/core/model/private/taskManager.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/core/model/private/taskManager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/core/model/templateInfo/private/defaultTemplates.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/core/model/templateInfo/private/templateSerialization.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/core/model/templateInfo/private/templateSerialization.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/core/model/templateInfo/server.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/core/model/templateInfo/server.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/core/model/model.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/core/model/model.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/core/toolkit/timerTask.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/core/toolkit/timerTask.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/core/core.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/core/core.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/toolkit/dataDownloader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/toolkit/dataDownloader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/toolkit/qssHelper.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/toolkit/qssHelper.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/toolkit/stepperWidget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/toolkit/stepperWidget.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/view/mainWindow/mainWindowView.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/view/mainWindow/mainWindowView.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/view/workspace/common/templateInfo/templateInfo.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/view/workspace/common/templateInfo/templateInfo.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/view/workspace/common/templateInfo/templateStep.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/view/workspace/common/templateInfo/templateStep.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/view/workspace/common/imageCacheManager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/view/workspace/common/imageCacheManager.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/view/workspace/common/modeZone.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/view/workspace/common/modeZone.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/view/workspace/modeMulti/projectZone/deviceWidget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/view/workspace/modeMulti/projectZone/deviceWidget.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/view/workspace/modeMulti/projectZone/projectZoneMulti.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/view/workspace/modeMulti/projectZone/projectZoneMulti.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/view/workspace/modeMulti/multiModeView.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/view/workspace/modeMulti/multiModeView.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/view/workspace/modeSingle/commander/commander.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/view/workspace/modeSingle/commander/commander.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/view/workspace/modeSingle/projectZone/projectZoneSingle.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/view/workspace/modeSingle/projectZone/projectZoneSingle.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/view/workspace/modeSingle/modeSingleView.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/view/workspace/modeSingle/modeSingleView.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/view/workspace/workspace.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/view/workspace/workspace.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

# Add config file to sources
set(FU_CONFIG_FILE "${CMAKE_CURRENT_BINARY_DIR}/config.h")
configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/config.h.in"
	"${FU_CONFIG_FILE}"
)
source_group("Source Files/res" FILES ${FU_CONFIG_FILE})
list(APPEND SOURCE_FILES "${FU_CONFIG_FILE}")

# Add resources file to sources
if(WIN32)
	set(RESOURCES_FILE "${CMAKE_CURRENT_SOURCE_DIR}/res/win/resources.rc")
	source_group("Source Files/res" FILES ${RESOURCES_FILE})
	list(APPEND SOURCE_FILES "${RESOURCES_FILE}")
elseif(APPLE)
    set(ICON_FILENAME "Icon.icns")
	set(ICON_FILEPATH "${CMAKE_CURRENT_SOURCE_DIR}/res/macos/${ICON_FILENAME}")
	set_source_files_properties(${ICON_FILEPATH} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
	source_group("Source Files/res" FILES ${ICON_FILEPATH})    
	list(APPEND SOURCE_FILES "${ICON_FILENAME}")
endif()
    
# Add Qt resource file to sources
set(QT_RESOURCE_FILES 
	"${CMAKE_CURRENT_SOURCE_DIR}/res/main.qrc"
	"${CMAKE_CURRENT_SOURCE_DIR}/res/main.qss")
source_group("Source Files/res" FILES ${QT_RESOURCE_FILES})
list(APPEND SOURCE_FILES ${QT_RESOURCE_FILES})

# Group all sources
commonQt_source_group("${SOURCE_FILES}"
	GENERATED_UI_HPP_FILES 
	GENERATED_HPP_CPPMOC_FILES
)

# Compute executable
commonQt_add_executable(${PROJECT_NAME} 
	"${SOURCE_FILES}"
	"${GENERATED_UI_HPP_FILES}"
	"${GENERATED_HPP_CPPMOC_FILES}"
)

# Setup Info/Icon properties
if(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE_ICON_FILE ${ICON_FILENAME}
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/res/macos/Info.plist.in
    )
endif()
    
# Include header directory
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Link and copy dlls
commonQt_finalize_executable(${PROJECT_NAME} "${QT_MODULES}")

# Set current target as active
if(WIN32 AND MSVC)
	set_directory_properties(PROPERTIES VS_STARTUP_PROJECT ${PROJECT_NAME})
endif()

# Copy qss close to executable (for live reloading)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/res/main.qss" $<TARGET_FILE_DIR:${PROJECT_NAME}>
)