set(MODULE_NAME "Scan")

set(MODULE_LIBS Motors
                ConfigurationFileParser
                XRaySensor
                spdlog::spdlog
                PostProcessing
                Devices
                UIManagementServer
)

set(INCLUDE_DIRS    ./include
)

set(SRC_FILES   ./src/ScanningHXP.cpp
                ./src/ScanningStepper.cpp
)

add_library(${MODULE_NAME} ${SRC_FILES})

target_include_directories(${MODULE_NAME} PUBLIC ${INCLUDE_DIRS})
target_link_libraries(${MODULE_NAME} PUBLIC ${MODULE_LIBS} )

#=========================================================

#=========================================================
#if(BUILD_Tests)
#    add_subdirectory(test)
#endif()
#=========================================================