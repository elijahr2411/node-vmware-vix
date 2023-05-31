# this is 100% a ugly hack, but it's a ugly hack which works
# Note that this doesn't:
# - link to the real product specific VIX library (there are goodies here)
# - link to vmware-base (LOTS of goodies.. should figure that out)
# No way to handle versioning yet, since VIXwrapper


include(FindPackageHandleStandardArgs)


find_library(VMWAREVIX_LIBRARY 
	NAMES
		vixAllProducts

	# this is linux only atm sorry
	PATHS
		/usr/lib
		/usr/lib/vmware-vix
)

find_path(VMWAREVIX_INCLUDE_DIR
	NAMES
		vix.h
	PATHS
		/usr/include/vmware-vix
)

find_package_handle_standard_args(VMwareVIX REQUIRED_VARS VMWAREVIX_LIBRARY VMWAREVIX_INCLUDE_DIR HANDLE_COMPONENTS)

if (VMWAREVIX_FOUND)
  mark_as_advanced(VMWAREVIX_INCLUDE_DIR)
  mark_as_advanced(VMWAREVIX_LIBRARY)
endif()

if (VMWAREVIX_FOUND AND NOT TARGET VMware::VIX)
  add_library(VMware::VIX SHARED IMPORTED)
  set_property(TARGET VMware::VIX PROPERTY IMPORTED_LOCATION ${VMWAREVIX_LIBRARY})
  target_include_directories(VMware::VIX INTERFACE ${VMWAREVIX_INCLUDE_DIR})
endif()
