# FindHXP.cmake

# HXP_FOUND:        TRUE if HXP is found.
# HXP_INCLUDE_DIRS: Include directories for HXP.
# HXP_LIBRARIES:    Libraries for all HXP component libraries and dependencies.
# HXP_LIBRARIES_DLL:Shared libraries for HXP component
# HXP_LIB:          Shared Library Target

macro(HXP_REPORT_NOT_FOUND REASON_MSG)
    unset(HXP_FOUND)
    unset(HXP_INCLUDE_DIRS)
    unset(HXP_LIBRARIES)
    if(HXP_FIND_QUIETLY)
        message(STATUS "Failed to find HXP - " ${REASON_MSG} ${ARGN})
    elseif(HXP_FIND_REQUIRED)
        message(FATAL_ERROR "Failed to find HXP - " ${REASON_MSG} ${ARGN})
    else()
        # Neither QUIETLY nor REQUIRED, use no priority which emits a message
        # but continues configuration and allows generation.
        message("-- Failed to find HXP - " ${REASON_MSG} ${ARGN})
    endif()
endmacro(HXP_REPORT_NOT_FOUND)

# First set, later unset if not found
set(HXP_FOUND TRUE)

find_path(HXP_INCLUDE_DIRS NAMES hxp_drivers.h
PATHS "${PROJECT_SOURCE_DIR}/extern/Hexapod/win_32")

if(NOT HXP_INCLUDE_DIRS OR NOT EXISTS ${HXP_INCLUDE_DIRS})
HXP_REPORT_NOT_FOUND("Could not find HXP directory")
else()
message(STATUS "HXP directory found: " ${HXP_INCLUDE_DIRS})
endif()

find_libraries_list(HXP_LIBRARIES ${HXP_INCLUDE_DIRS} HXP_C8_drivers.lib)

if(NOT HXP_LIBRARIES OR NOT EXISTS ${HXP_LIBRARIES})
HXP_REPORT_NOT_FOUND("Could not find HXP library")
else()
message(STATUS "HXP library found: " ${HXP_LIBRARIES})
endif()

find_dll_libraries_list(HXP_LIBRARIES_DLL ${HXP_INCLUDE_DIRS} HXP_C8_drivers.dll)

if(NOT HXP_LIBRARIES_DLL OR NOT EXISTS ${HXP_LIBRARIES_DLL})
    HXP_REPORT_NOT_FOUND("Could not find HXP Shared library")
else()
    message(STATUS "HXP Shared library found: " ${HXP_LIBRARIES_DLL})
endif()

if(HXP_FOUND)
    #Setup library
    add_library(HXP_LIB SHARED IMPORTED)
    set_property(TARGET HXP_LIB PROPERTY
    IMPORTED_LOCATION "${HXP_LIBRARIES_DLL}")
    set_property(TARGET HXP_LIB PROPERTY
    IMPORTED_IMPLIB "${HXP_LIBRARIES}")
    target_include_directories(HXP_LIB INTERFACE "${HXP_INCLUDE_DIRS}")
endif()

