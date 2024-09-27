#! setup_dll_postbuild : Function to transfer TARGET runtime dependency dlls to the target file directory
#
# Takes a TARGET as input and adds to the executable directory associated to the target
# the runtime dependency libraries
#
# \param:TARGET specify the target in which to install the runtime dll dependencies
#

function(setup_dll_postbuild)
    set(options)
    set(oneValueArgs TARGET)
    set(multiValueArgs)
    cmake_parse_arguments(SETUP_DLL_POSTBUILD "${options}" "${oneValueArgs}"
                        "${multiValueArgs}" ${ARGN} )

    message(STATUS "Setting up POST-BUILD configuration for Target: ${SETUP_DLL_POSTBUILD_TARGET}")
    add_custom_command(TARGET ${SETUP_DLL_POSTBUILD_TARGET} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_RUNTIME_DLLS:${SETUP_DLL_POSTBUILD_TARGET}>" $<TARGET_FILE_DIR:${SETUP_DLL_POSTBUILD_TARGET}>
    COMMAND_EXPAND_LISTS
    )

endfunction()


#=================================================================================

#! find_libraries_list : Function to find a list of libraries in a specified path
#
# Looks for LIBRARY_NAMES in the BASE_PATH and results in a list of 
# absolute paths for each LIBRARY_NAME. It generates a FATAL_ERROR if the library is not found
#
# \param:BASE_PATH path to look for the library
# \group:LIST_RESULT is the output of the function
# \group:LIBRARY_NAMES list of names to look for
#

function(find_libraries_list LIST_RESULT BASE_PATH LIBRARY_NAMES)
    
    foreach(LIB_NAME IN LISTS LIBRARY_NAMES)
        unset(TEMP_PATH CACHE)
        message(STATUS "Finding library name: ${LIB_NAME}")
        find_library(TEMP_PATH NAMES ${LIB_NAME}
                               PATHS ${BASE_PATH}
                               )

        if(NOT TEMP_PATH)
            message(FATAL_ERROR "LIBRARY NOT FOUND IN ${BASE_PATH}: ${LIB_NAME}")
        endif()                   

        list(APPEND TEMP_LIST "${TEMP_PATH}")

    endforeach()
    
    SET(${LIST_RESULT} ${TEMP_LIST} PARENT_SCOPE)

endfunction()

#=====================================================================================================

#! find_dll_libraries_list : Function to find a list of dll libraries in a specified path
#
# Looks for dll LIBRARY_NAMES in the BASE_PATH and results in a list of 
# absolute paths for each LIBRARY_NAME. It generates a FATAL_ERROR if the library is not found
#
# \param:BASE_PATH path to look for the library
# \group:LIST_RESULT is the output of the function
# \group:LIBRARY_NAMES list of names to look for
#

function(find_dll_libraries_list LIST_RESULT BASE_PATH LIBRARY_NAMES)

    foreach(LIB_NAME IN LISTS LIBRARY_NAMES)
        get_filename_component(LIB_EXT ${LIB_NAME} EXT)
        if(NOT "${LIB_EXT}" STREQUAL ".dll")
            message(FATAL_ERROR "${LIB_EXT} is not a .dll file. Please use find_libraries_list() function instead for libraries")
        endif()
    endforeach()

    
    foreach(LIB_NAME IN LISTS LIBRARY_NAMES)
        unset(TEMP_PATH CACHE)
        message(STATUS "Finding shared library name: ${LIB_NAME}")
        find_file(TEMP_PATH NAMES ${LIB_NAME}
                            PATHS ${BASE_PATH}
                            )

        if(NOT TEMP_PATH)
            message(FATAL_ERROR "LIBRARY NOT FOUND IN ${BASE_PATH}: ${LIB_NAME}")
        endif()                   

        list(APPEND TEMP_LIST "${TEMP_PATH}")

    endforeach()
    
    SET(${LIST_RESULT} ${TEMP_LIST} PARENT_SCOPE)

endfunction()


#=====================================================================================================

#sample of new function (to be implemented)
# find_libraries_list_test(OUTPUT HXP_LIBRARIES_DLL 
#                         BASE_PATH "${HXP_INCLUDE_DIRS}" 
#                         NAMES "${HXP_LIBRARIES_DLL}" 
#                         DLL TRUE) 

#=====================================================================================================

#! package_add_test_with_libraries : Function to add tests in module
#
# Function to add google tests with specified libraries and option to include shared dlls 
# It links with gtest, gmock and gtest_main.
#
# \param:ADD_DLL    BOOLEAN conditional to add DLL shared libraries required for TESTNAME
# \param:TESTNAME   Name of test
# \group:FILES      Source files for the tests
# \group:LIBRARIES  Extra libraries required to run the tests
#

function(package_add_test_with_libraries)
    set(options ADD_DLL)
    set(oneValueArgs TESTNAME)
    set(multiValueArgs FILES LIBRARIES)

    cmake_parse_arguments(ADD_TEST "${options}" "${oneValueArgs}"
                        "${multiValueArgs}" ${ARGN} )

    #Set google libraries to be used for the tests
    set(_GOOGLE_LIB_TEST gtest
                         gmock
                         gtest_main)

    add_executable(${ADD_TEST_TESTNAME} "${ADD_TEST_FILES}")

    target_link_libraries(${ADD_TEST_TESTNAME}
                          ${_GOOGLE_LIB_TEST}
                          ${ADD_TEST_LIBRARIES})

    # gtest_discover_tests replaces gtest_add_tests
    gtest_discover_tests(${ADD_TEST_TESTNAME}
        # set a working directory to your project root so that you can find test data via paths relative to the project root
        WORKING_DIRECTORY ${PROJECT_DIR}
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
    )

    set_target_properties(${ADD_TEST_TESTNAME} PROPERTIES FOLDER tests)

    #Add DLL libraries to test for windows dependcies
    if(ADD_TEST_ADD_DLL)
        setup_dll_postbuild(TARGET ${ADD_TEST_TESTNAME})
    endif()

endfunction()

#=====================================================================================================

#macro for adding test. This is for debugging

macro(package_add_test TESTNAME)
    # create an exectuable in which the tests will be stored
    
    add_executable(${TESTNAME} ${ARGN})
    # link the Google test infrastructure, mocking library, and a default main fuction to
    # the test executable.  Remove g_test_main if writing your own main function.
    target_link_libraries(${TESTNAME} gtest gmock gtest_main)
    # gtest_discover_tests replaces gtest_add_tests,
    # see https://cmake.org/cmake/help/v3.10/module/GoogleTest.html for more options to pass to it
    gtest_discover_tests(${TESTNAME}
        # set a working directory to your project root so that you can find test data via paths relative to the project root
        WORKING_DIRECTORY ${PROJECT_DIR}
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
    )
    set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)

    setup_dll_postbuild(TARGET ${TESTNAME})
    
endmacro()