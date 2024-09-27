set(MODULE_NAME "Devices")

set(MODULE_LIBS Motors
                ConfigurationFileParser
                XRaySensor
                spdlog::spdlog
                sml
                PostProcessing
                Communication
                Sensors
                json
                Scan
)

set(INCLUDE_DIRS    ./include/DeviceFactory
                    ./include/Autocollimator
                    ./include/Crystal
                    ./include/Monochromator
                    ./include/Slit
                    ./include/XRaySensor
                    ./include/XRaySource
                    ./include/
)

set(SRC_FILES   ./src/DeviceFactory/XRayMachineDevicesFactory.cpp
                ./src/Autocollimator/Actions.cpp ./src/Autocollimator/AutocollimatorDeviceController.cpp
                ./src/Crystal/Actions.cpp  ./src/Crystal/CrystalDeviceController.cpp          
                ./src/Monochromator/Actions.cpp   ./src/Monochromator/MonochromatorDeviceController.cpp
                ./src/Slit/Actions.cpp ./src/Slit/SlitDeviceController.cpp
                ./src/XRaySensor/Actions.cpp ./src/XRaySensor/XRaySensorDeviceController.cpp
                ./src/XRaySource/Actions.cpp ./src/XRaySource/XRaySourceDeviceController.cpp
)

add_library(${MODULE_NAME} ${SRC_FILES})

target_include_directories(${MODULE_NAME} PUBLIC ${INCLUDE_DIRS})
target_link_libraries(${MODULE_NAME} PUBLIC ${MODULE_LIBS} )

#=========================================================
add_executable(fsmsample_crystal ./samples/fsmsample_crystal.cpp)
target_link_libraries(fsmsample_crystal ${MODULE_NAME} ${MODULE_LIBS})

add_executable(fsmsample_xRaySource ./samples/fsmsample_xRaySource.cpp)
target_link_libraries(fsmsample_xRaySource ${MODULE_NAME} ${MODULE_LIBS})
#=========================================================
if(BUILD_Tests)
    add_subdirectory(test)
endif()
#=========================================================