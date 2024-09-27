# Helper functions for setting up external repositories from git
#=====================================================================================================

#Function to setup the google test repository based on TAG input

function(setup_gtest_repository TAG)

    message(STATUS "Setting up google test repository version: ${TAG}")

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG ${TAG} #checkout specified tag
    )

    if (WIN32)
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE) #Required for WINDOWS OS linking.
        option(gtest_disable_pthreads "Disable uses of pthreads in gtest." ON) #removes the warning in cmake
    endif (WIN32)

endfunction()

#Function to setup the spdlog repository based on TAG input

function(setup_spdlog_repository TAG)

    message(STATUS "Setting up spdlog repository version: ${TAG}")

    FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG ${TAG} #checkout specified tag
    )

endfunction()


#=====================================================================================================