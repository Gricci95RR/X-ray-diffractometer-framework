# FindXIMC.cmake

# XIMC_FOUND:        TRUE if XIMC is found.
# XIMC_INCLUDE_DIRS: Include directories for XIMC.
# XIMC_LIBRARIES:    Libraries for all XIMC component libraries and dependencies.
# XIMC_LIBRARIES_DLL:Shared libraries for XIMC component
# XIMC_LIB:          Shared Library Target

macro(XIMC_REPORT_NOT_FOUND REASON_MSG)
    unset(XIMC_FOUND)
    unset(XIMC_INCLUDE_DIRS)
    unset(XIMC_LIBRARIES)
    if(XIMC_FIND_QUIETLY)
        message(STATUS "Failed to find XIMC - " ${REASON_MSG} ${ARGN})
    elseif(XIMC_FIND_REQUIRED)
        message(FATAL_ERROR "Failed to find XIMC - " ${REASON_MSG} ${ARGN})
    else()
        # Neither QUIETLY nor REQUIRED, use no priority which emits a message
        # but continues configuration and allows generation.
        message("-- Failed to find XIMC - " ${REASON_MSG} ${ARGN})
    endif()
endmacro(XIMC_REPORT_NOT_FOUND)

# First set, later unset if not found
set(XIMC_FOUND TRUE)

find_path(XIMC_INCLUDE_DIRS NAMES ximc.h
                            PATHS "${PROJECT_SOURCE_DIR}/extern/StandaController/win_32")

if(NOT XIMC_INCLUDE_DIRS OR NOT EXISTS ${XIMC_INCLUDE_DIRS})
    XIMC_REPORT_NOT_FOUND("Could not find XIMC directory")
else()
    message(STATUS "XIMC directory found: " ${XIMC_INCLUDE_DIRS})
endif()

find_libraries_list(XIMC_LIBRARIES "${XIMC_INCLUDE_DIRS}" libximc.lib)

if(NOT XIMC_LIBRARIES OR NOT EXISTS ${XIMC_LIBRARIES})
    XIMC_REPORT_NOT_FOUND("Could not find XIMC library")
else()
    message(STATUS "XIMC library found: " ${XIMC_LIBRARIES})
endif()

SET(XIMC_SHARED_LIBRARIES_NAMES libximc.dll
                                bindy.dll
                                xiwrapper.dll
                                libjximc.dll)

find_dll_libraries_list(XIMC_LIBRARIES_DLL "${XIMC_INCLUDE_DIRS}" "${XIMC_SHARED_LIBRARIES_NAMES}")

foreach(X IN LISTS XIMC_LIBRARIES_DLL)
    message(STATUS "XIMC Shared library found: = ${X}")
endforeach()

if (XIMC_FOUND)
    #Create library
    add_library(XIMC_LIB SHARED IMPORTED)
    set_property(TARGET XIMC_LIB PROPERTY
            IMPORTED_LOCATION "${XIMC_LIBRARIES_DLL}")
    set_property(TARGET XIMC_LIB PROPERTY
            IMPORTED_IMPLIB "${XIMC_LIBRARIES}")

    target_include_directories(XIMC_LIB INTERFACE "${XIMC_INCLUDE_DIRS}")
endif()



