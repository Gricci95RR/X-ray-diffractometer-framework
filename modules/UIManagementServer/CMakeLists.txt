set(MODULE_NAME "UIManagementServer")

set(MODULE_LIBS Communication
                ConfigurationFileParser
                Devices
                Motors
                PostProcessing
                Scan
                Sensors
                XRaySensor
                json
                spdlog::spdlog
)

set(INCLUDE_DIRS    ./include
)

set(SRC_FILES   ./src/UIManagementServer.cpp
)

add_library(${MODULE_NAME} ${SRC_FILES})

target_include_directories(${MODULE_NAME} PUBLIC ${INCLUDE_DIRS})
target_link_libraries(${MODULE_NAME} PUBLIC ${MODULE_LIBS} )

#=========================================================
set(EXECUTABLE_SOURCES ${PROJECT_SOURCE_DIR}/modules/UIManagementServer/samples/gui_test.cpp)
set(EXECUTABLE_NAME gui_test)
add_executable(${EXECUTABLE_NAME} ${EXECUTABLE_SOURCES})
target_link_libraries(${EXECUTABLE_NAME} PUBLIC ${MODULE_NAME})
setup_dll_postbuild(TARGET ${EXECUTABLE_NAME})
#=========================================================
#if(BUILD_Tests)
#    add_subdirectory(test)
#endif()
#=========================================================