cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)

# Add common funtions folder
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/ext/cmake)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Include Qt common functions
include(commonQt)

# Project base information
project(firmwareUpdater VERSION 1.0.0 LANGUAGES CXX)

# Setup Qt project
commonQt_setup_qt()

# Set project flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add project source files
set(SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/src/core/common/type.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/core/controller/controller.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/core/controller/controller.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/core/controller/viewInterface.hpp
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
commonQt_source_group("${SOURCE_FILES}" GENERATED_UI_HPP_FILES GENERATED_HPP_CPPMOC_FILES)

# Compute executable
add_executable(${CMAKE_PROJECT_NAME} WIN32
	${SOURCE_FILES}
	${GENERATED_UI_HPP_FILES}
	${GENERATED_HPP_CPPMOC_FILES}
)

#include Include current header directory
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Set current target as active
set_directory_properties(PROPERTIES VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})

# Link and copy dlls
commonQt_target_link(${PROJECT_NAME})
