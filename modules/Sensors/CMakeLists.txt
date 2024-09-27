set(MODULE_NAME "Sensors")

set(MODULE_LIBS Motors
                ConfigurationFileParser
                XRaySensor
                spdlog::spdlog
                PostProcessing
                Devices
)

set(INCLUDE_DIRS    ./include
)

set(SRC_FILES   ./src/Sensors.cpp
)

add_library(${MODULE_NAME} ${SRC_FILES})

target_include_directories(${MODULE_NAME} PUBLIC ${INCLUDE_DIRS})
target_link_libraries(${MODULE_NAME} PUBLIC ${MODULE_LIBS} )

#=========================================================s
# Executable file (Samples)
set(EXECUTABLE_SOURCES ${PROJECT_SOURCE_DIR}/modules/Sensors/samples/sensorsSample.cpp)
set(EXECUTABLE_NAME sensorsSample)
add_executable(${EXECUTABLE_NAME} ${EXECUTABLE_SOURCES})
target_link_libraries(${EXECUTABLE_NAME} PUBLIC ${MODULE_NAME})

#=========================================================
#if(BUILD_Tests)
#    add_subdirectory(test)
#endif()
#=========================================================