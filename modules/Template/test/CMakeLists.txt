
#Name of the test
set(This ExampleTests)

#Name of source files
set(Sources
    ExampleTests.cpp
)

#===========================================
# Sample for using custom function
#===========================================

#package_add_test(ExampleTests ${Sources}) #macro for testing

package_add_test_with_libraries(TESTNAME ${This}
                                FILES "${Sources}"
                                LIBRARIES Template
                                ADD_DLL TRUE)

#===========================================
#===========================================


# Another approach of adding tests
#===========================================
# add_executable(${This} ${Sources})

# target_link_libraries(${This} PUBLIC 
#     gtest_main
#     Template
# )

# setup_dll_postbuild(TARGET ${This})

# add_test(
#     NAME ${This}
#     COMMAND ${This}
# )
#===========================================