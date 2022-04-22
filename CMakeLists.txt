cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)

# Project base information
project(firmwareUpdater VERSION 1.0.3.0 LANGUAGES CXX)

# Project properties
set(FU_VERSION_MAJ      ${PROJECT_VERSION_MAJOR})
set(FU_VERSION_MIN      ${PROJECT_VERSION_MINOR})
set(FU_VERSION_SUB      ${PROJECT_VERSION_PATCH})
set(FU_VERSION_REV      ${PROJECT_VERSION_TWEAK})

# Add common funtions folder
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/ext/cmake)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set project flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add global CMake preprocessor definition
# Used to get build system source in firmwareUpdaterConfig.h.in
add_definitions(-DCMAKE_FLAGS_USED)

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
	${CMAKE_CURRENT_SOURCE_DIR}/src/core/controller/viewInterface.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/core/model/private/defaultTemplates.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/core/model/private/templatesSerialization.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/core/model/private/templatesSerialization.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/core/model/server.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/core/model/server.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/core/toolkit/timerTask.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/core/toolkit/timerTask.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/toolkit/dataDownloader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/toolkit/dataDownloader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/toolkit/stepperWidget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/toolkit/stepperWidget.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/view/main/mainView.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/view/main/mainView.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/view/templateBrowser/templateBrowserView.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/view/templateBrowser/templateBrowserView.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/view/templateInfo/templateInfoView.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/view/templateInfo/templateInfoView.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/view/templateInfo/templateStepView.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/view/templateInfo/templateStepView.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/view/welcome/welcomeView.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui/view/welcome/welcomeView.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

# Add config file to sources
set(FU_CONFIG_FILE "${CMAKE_CURRENT_BINARY_DIR}/firmwareUpdaterConfig.h")
configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/firmwareUpdaterConfig.h.in"
	"${CMAKE_CURRENT_BINARY_DIR}/firmwareUpdaterConfig.h"
)
list(APPEND SOURCE_FILES "${FU_CONFIG_FILE}")

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

# Include header directory
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Link and copy dlls
commonQt_finalize_executable(${PROJECT_NAME} "${QT_MODULES}")

# Set current target as active
if(WIN32 AND MSVC)
    set_directory_properties(PROPERTIES VS_STARTUP_PROJECT ${PROJECT_NAME})
endif()