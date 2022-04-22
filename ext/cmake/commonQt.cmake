# Avoid multi inclusion of this file
if(COMMON_QT_CMAKE_INCLUDED)
   return()
endif()
set(COMMON_QT_CMAKE_INCLUDED true)

#include cmake common functions
include(common)

# Setup Qt
macro(commonQt_setup_qt QT_PACKAGE_NAME QT_MODULES)
	# Enable automatic Qt precompiler
	set(CMAKE_AUTOMOC ON)
	set(CMAKE_AUTORCC ON)
	set(CMAKE_AUTOUIC ON)

	# Find Qt and required components
	find_package(${QT_PACKAGE_NAME} COMPONENTS ${QT_MODULES} REQUIRED)
    
    # Ignore migration compilation warnings if not on Qt 6
    # Allows to silent warnings like "C5054: operator '&': 
    # deprecated between enumerations of different types"
    if(WIN32 AND MSVC AND ${QT_VERSION_MAJOR} LESS 6)
        add_compile_options(/Wv:18)
    endif()
endmacro()

# Create source group for the given qt source and generated files
# SOURCE_FILES The list of source files to add
# GENERATED_UI_HPP_FILES hpp files generated from ui source files
# GENERATED_HPP_CPPMOC_FILES moc cpp files generated from hpp source files
macro(commonQt_source_group SOURCE_FILES GENERATED_UI_HPP_FILES GENERATED_HPP_CPPMOC_FILES)
	# source files group
	common_source_group("Source Files" "${SOURCE_FILES}" "${CMAKE_CURRENT_SOURCE_DIR}/src")

	# Generate hpp from ui and group them
	set(SOURCE_UI_FILES ${SOURCE_FILES})
	list(FILTER SOURCE_UI_FILES INCLUDE REGEX ".ui$")
	if(NOT "${SOURCE_UI_FILES}" STREQUAL "")
		qt_wrap_ui(${GENERATED_UI_HPP_FILES} ${SOURCE_UI_FILES})
		common_source_group("Generated Files" "${${GENERATED_UI_HPP_FILES}}" "${CMAKE_CURRENT_BINARY_DIR}")
	endif()

	# Generate cpp moc from hpp and group them
	set(SOURCE_HPP_FILES ${SOURCE_FILES})
	list(FILTER SOURCE_HPP_FILES INCLUDE REGEX ".hpp$")
	if(NOT "${SOURCE_HPP_FILES}" STREQUAL "")
		qt_wrap_cpp(${GENERATED_HPP_CPPMOC_FILES} ${SOURCE_HPP_FILES})
		common_source_group("Generated Files" "${${GENERATED_HPP_CPPMOC_FILES}}" "${CMAKE_CURRENT_BINARY_DIR}/src")	
	endif()
endmacro()

function(commonQt_add_executable TARGET_NAME 
    SOURCE_FILES GENERATED_UI_HPP_FILES GENERATED_HPP_CPPMOC_FILES)
    if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
        qt_add_executable(${TARGET_NAME}
            MANUAL_FINALIZATION
            ${SOURCE_FILES}
            ${GENERATED_UI_HPP_FILES}
            ${GENERATED_HPP_CPPMOC_FILES}
        )
    else()
        add_executable(${TARGET_NAME}
            ${SOURCE_FILES}
            ${GENERATED_UI_HPP_FILES}
            ${GENERATED_HPP_CPPMOC_FILES}
        )
    endif()
endfunction()


# Finalize last steps to make executable ready (link, dependencies setup, dlls copy...) 
function(commonQt_finalize_executable TARGET_NAME QT_MODULES)
	# Link executable to needed libraries
    foreach(QT_MODULE ${QT_MODULES})
        target_link_libraries(${TARGET_NAME} PRIVATE Qt::${QT_MODULE})
    endforeach(QT_MODULE)

    # Setup network dependencies
    if("Network" IN_LIST QT_MODULES)
        # Setup Openssl needed for network requests.
        # Note that OpenSSL can be installed as an optional Qt component
        # by selecting "OpenSSL Toolkit".
        if (NOT EXISTS ${QT_ROOT_DIR})
            message(FATAL_ERROR "Variable QT_ROOT_DIR not found. Please set it to your Qt installation dir.")
        endif()
        set(OPENSSL_ROOT_DIR "${QT_ROOT_DIR}/Tools/OpenSSL/Win_x86")
        common_setup_openssl(${TARGET_NAME})
    endif()
    
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
    )

    if(QT_VERSION_MAJOR EQUAL 6)
        qt_finalize_executable(${PROJECT_NAME})
    endif()
    
    commonQt_deploy_executable(${PROJECT_NAME})
endfunction()

function(commonQt_deploy_executable TARGET_NAME)
    # Retrieve the absolute path to qmake and then use that path to find the binaries
    get_target_property(QMAKE_EXECUTABLE Qt::qmake IMPORTED_LOCATION)
    get_filename_component(QMAKE_BIN_DIR "${QMAKE_EXECUTABLE}" DIRECTORY)
    
    # Find and run deployment binary
    if(WIN32)
        find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${QMAKE_BIN_DIR}")
        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E env PATH="${QMAKE_BIN_DIR}" "${WINDEPLOYQT_EXECUTABLE}" 
                "$<TARGET_FILE:${TARGET_NAME}>"
            COMMENT "Running windeployqt..."
        )
    else(APPLE)
        find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${QMAKE_BIN_DIR}")
        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND "${MACDEPLOYQT_EXECUTABLE}" 
                "$<TARGET_FILE_DIR:${TARGET_NAME}>/../.."
                -always-overwrite
            COMMENT "Running macdeployqt..."
        )
    endif()
endfunction()