add_library(vmware-vix SHARED
	${CMAKE_JS_SRC}
	vmware.cpp
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

set_target_properties(vmware-vix PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
)


if(MSVC AND CMAKE_JS_NODELIB_DEF AND CMAKE_JS_NODELIB_TARGET)
  # Generate node.lib
  execute_process(COMMAND ${CMAKE_AR} /def:${CMAKE_JS_NODELIB_DEF} /out:${CMAKE_JS_NODELIB_TARGET} ${CMAKE_STATIC_LINKER_FLAGS})
endif()

