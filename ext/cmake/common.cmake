# Avoid multi inclusion of this file
if(COMMON_CMAKE_INCLUDED)
   return()
endif()
set(COMMON_CMAKE_INCLUDED true)

# Set the warnings level to max (including warnings as error)
function(set_warnings_level_max)
    if(WIN32 AND MSVC)
        # W4 is enough on Windows (too much noise with Wall)
		add_compile_options(/W4 /WX)
	elseif(APPLE)
		add_compile_options(-Wall -Werror -g)
	endif()
endfunction(set_warnings_level_max)

# Create source group following the same project structure
# SOURCE_GROUP_PREFIXNAME SubGroups to prefix to the generated group name structure
# SOURCE_FILES The list of source files to group (paths can be either relative to current dir, or absolute)
# BASE_DIR (optional) The base directory containing the source files to group
# Ex: helper_source_group("Private\\Sources" "${CMAKE_CURRENT_BINARY_DIR}" "files1.cpp;files2.cpp")
macro(common_source_group SOURCE_GROUP_PREFIXNAME SOURCE_FILES)
	if("${SOURCE_GROUP_PREFIXNAME}" STREQUAL "" OR "${SOURCE_FILES}" STREQUAL "")
		message(FATAL_ERROR "one of the inputs is empty")
	endif()

	# Update base directory with given optional value
	set(BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
	# Cannot use ARGN directly with list() command. Copy to a variable first.
	set (EXTRA_ARGS ${ARGN})
	# Did we get any optional args?
	list(LENGTH EXTRA_ARGS COUNT_EXTRA_ARGS)
	if(${COUNT_EXTRA_ARGS} GREATER 0)
		list(GET EXTRA_ARGS 0 BASE_DIR)
	endif()

	foreach(FILE_PATH ${SOURCE_FILES})
		# Always start with absolute path
		get_filename_component(ABSOLUTE_FILE_PATH ${FILE_PATH} ABSOLUTE)
		# Get relative path (to base dir)
		file(RELATIVE_PATH FILE_PATH ${BASE_DIR} ${ABSOLUTE_FILE_PATH})
		# Extract file name/directory
		get_filename_component(FILE_NAME ${FILE_PATH} NAME)
		get_filename_component(FILE_DIRECTORY ${FILE_PATH} DIRECTORY)
		if("${FILE_NAME}" STREQUAL "")
			message(FATAL_ERROR "Bad file format")
		endif()

		# Get hierarchical structure to build source_groups
		set(FULL_GROUP_NAME "${SOURCE_GROUP_PREFIXNAME}")
		if(NOT "${FILE_DIRECTORY}" STREQUAL "")
			string(REPLACE "/" "\\" GROUP_NAME ${FILE_DIRECTORY})
			set(FULL_GROUP_NAME "${SOURCE_GROUP_PREFIXNAME}\\\\${GROUP_NAME}")
		endif()
		source_group("${FULL_GROUP_NAME}" FILES ${ABSOLUTE_FILE_PATH})
	endforeach()
endmacro()

function(common_setup_openssl TARGET_NAME )
    find_package(OpenSSL REQUIRED)
    if(NOT DEFINED OPENSSL_ROOT_DIR)
        message(FATAL_ERROR "OPENSSL_ROOT_DIR is not set")    
    elseif(OPENSSL_FOUND)
        message(STATUS "Found OpenSSL ${OPENSSL_VERSION}")
        target_include_directories(${TARGET_NAME} PUBLIC ${OPENSSL_INCLUDE_DIR})        
        target_link_libraries(${TARGET_NAME} PRIVATE OpenSSL::SSL OpenSSL::Crypto)
		file(GLOB DLLS_PATHS ${OPENSSL_ROOT_DIR}/bin/*.dll) 
        foreach(DLLS_PATH ${DLLS_PATHS})
            add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${DLLS_PATH}" $<TARGET_FILE_DIR:${TARGET_NAME}>
            )
        endforeach()
    endif()
endfunction()