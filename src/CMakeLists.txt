add_library(vmware-vix SHARED
	${CMAKE_JS_SRC}
	vmware.cpp
)

target_compile_features(vmware-vix PUBLIC
	cxx_std_20
)

set_target_properties(vmware-vix PROPERTIES PREFIX "" SUFFIX ".node")

target_include_directories(vmware-vix PRIVATE
	${CMAKE_JS_INC}
)

target_link_libraries(vmware-vix 
	${CMAKE_JS_LIB}
	VMware::VIX
	NAPI::napi
)

if(WIN32)
	target_link_libraries(vmware-vix
		kernel32
		user32
		advapi32
		ole32
		oleaut32
		ws2_32 # oh dear
		shell32
	)
	target_link_options(vmware-vix
		PUBLIC "/nodefaultlib:libcmt.lib")
endif()

# doesnt seem to do anything because cmake-js plays heavy-handed
#set_target_properties(vmware-vix PROPERTIES
#	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}
#)


# Generate node.lib
if(MSVC AND CMAKE_JS_NODELIB_DEF AND CMAKE_JS_NODELIB_TARGET)
  execute_process(COMMAND ${CMAKE_AR} /def:${CMAKE_JS_NODELIB_DEF} /out:${CMAKE_JS_NODELIB_TARGET} ${CMAKE_STATIC_LINKER_FLAGS})
endif()

