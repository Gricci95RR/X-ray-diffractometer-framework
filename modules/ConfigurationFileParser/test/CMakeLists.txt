#Name of the test
set(This TestConfigurationFileParser)

#Name of source files
set(Configuration_TESTS_FILES 
                        main.cpp
                        TestConfiguration.cpp
                        TestReadCrystalParameters.cpp
                        TestReadMonoChromatorParameters.cpp
                        TestReadXRaySourceParameters.cpp
)

#===========================================
add_executable(${This} ${Configuration_TESTS_FILES})

target_link_libraries(${This} PUBLIC 
    gtest_main
    gmock
    ConfigurationFileParser
)

# setup_dll_postbuild(TARGET ${This})

add_test(
    NAME ${This}
    COMMAND ${This}
 )
#===========================================