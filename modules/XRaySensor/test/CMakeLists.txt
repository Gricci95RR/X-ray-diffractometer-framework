#Name of the test
set(This XRaySensorTest)

#Name of source files
set(Motors_TESTS_FILES 
                    main.cpp
)

#===========================================
add_executable(${This} ${Motors_TESTS_FILES})

target_link_libraries(${This} PUBLIC 
    gtest_main
    gmock
    ConfigurationFileParser
    Motors
)

setup_dll_postbuild(TARGET ${This})
add_test(
    NAME ${This}
    COMMAND ${This}
)
#===========================================
