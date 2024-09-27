#Name of the test
set(This DevicesTest)

#Name of source files
set(Devices_TESTS_FILES 
                    main.cpp
                    CrystalDeviceTest.cpp
                    MonochromatorDeviceTest.cpp
                    SlitDeviceTest.cpp
                    AutocollimatorDeviceTest.cpp
                    XRaySourceDeviceTest.cpp
)

#===========================================
add_executable(${This} ${Devices_TESTS_FILES})

target_link_libraries(${This} PUBLIC 
    gtest_main
    gmock
    ConfigurationFileParser
    Motors
    Devices
    XRaySensor
    spdlog::spdlog
    sml
    PostProcessing
)

setup_dll_postbuild(TARGET ${This})
add_test(
    NAME ${This}
    COMMAND ${This}
)
#===========================================