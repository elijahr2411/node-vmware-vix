# this is 100% a ugly hack, but it's a ugly hack which works
# Note that this doesn't:
# - link to the real product specific VIX library (there are goodies here)
# - link to vmware-base (LOTS of goodies.. should figure that out)
# No way to handle versioning yet, since VIXwrapper


include(FindPackageHandleStandardArgs)

if(WIN32)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(__VMWAREVIX_LIBRARY_NAME "Vix64AllProducts")
	else()
		set(__VMWAREVIX_LIBRARY_NAME "VixAllProducts")
	endif()
else()
	# the library name is sane everywhere else
	set(__VMWAREVIX_LIBRARY_NAME "vixAllProducts")
endif()

find_library(VMWAREVIX_LIBRARY 
	NAMES
		${__VMWAREVIX_LIBRARY_NAME}

	PATHS
		/usr/lib
		/usr/lib/vmware-vix
		"C:/Program Files (x86)/VMware/VMware VIX"
		
)

find_path(VMWAREVIX_INCLUDE_DIR
	NAMES
		vix.h
	PATHS
		/usr/include/vmware-vix
		"C:/Program Files (x86)/VMware/VMware VIX"
)

find_package_handle_standard_args(VMwareVIX REQUIRED_VARS VMWAREVIX_LIBRARY VMWAREVIX_INCLUDE_DIR HANDLE_COMPONENTS)

if (VMWAREVIX_FOUND)
  mark_as_advanced(VMWAREVIX_INCLUDE_DIR)
  mark_as_advanced(VMWAREVIX_LIBRARY)
endif()

if (VMWAREVIX_FOUND AND NOT TARGET VMware::VIX)
	add_library(VMware::VIX SHARED IMPORTED)
	if(WIN32)
		# Windows requires IMPORTED_IMPLIB to be set
		# (IMPORTED_LOCATION would be set to the DLL path, but we don't care about that)
		set_property(TARGET VMware::VIX PROPERTY 
			IMPORTED_IMPLIB ${VMWAREVIX_LIBRARY}
		)
  	else()
  		set_property(TARGET VMware::VIX PROPERTY 
			IMPORTED_LOCATION ${VMWAREVIX_LIBRARY}
		)
	endif()
	target_include_directories(VMware::VIX INTERFACE ${VMWAREVIX_INCLUDE_DIR})
endif()
