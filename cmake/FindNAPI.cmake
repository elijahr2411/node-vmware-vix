include(FindPackageHandleStandardArgs)

# This doesn't spark joy but whatever at least I can relegate it to here
# and keep the uglyness in a more maintainable spot lol
execute_process(COMMAND node -p "require('node-addon-api').include"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE NAPI_INCLUDE_DIR
        )
string(REGEX REPLACE "[\r\n\"]" "" NAPI_INCLUDE_DIR ${NAPI_INCLUDE_DIR})

find_package_handle_standard_args(NAPI REQUIRED_VARS NAPI_INCLUDE_DIR HANDLE_COMPONENTS)

if(NAPI_FOUND)
	mark_as_advanced(NAPI_INCLUDE_DIR)
endif()

if(NAPI_FOUND AND NOT TARGET NAPI::napi)
	add_library(NAPI::napi INTERFACE IMPORTED)
	target_compile_definitions(NAPI::napi INTERFACE
		-DNAPI_VERSION=6
	)
	target_include_directories(NAPI::napi INTERFACE ${NAPI_INCLUDE_DIR})
endif()



