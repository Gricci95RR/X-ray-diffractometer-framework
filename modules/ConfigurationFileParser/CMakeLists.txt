set(MODULE_NAME "ConfigurationFileParser")

set(INCLUDE_DIRS ./include)

set(SRC_FILES ./src/Configuration.cpp
              ./include/ConfigurationMock.hpp
              ./include/ConfigurationMockConfig.hpp
)

set(MODULE_LIBS ini
                spdlog::spdlog
                gtest_main
                gmock
)

add_library(${MODULE_NAME} ${SRC_FILES})
target_include_directories(${MODULE_NAME} PUBLIC ${INCLUDE_DIRS})
target_link_libraries(${MODULE_NAME} PUBLIC ${MODULE_LIBS})

#=========================================================
# Executable file (Samples)
set(EXECUTABLE_SOURCES ${PROJECT_SOURCE_DIR}/modules/ConfigurationFileParser/samples/SampleConfiguration.cpp)
set(EXECUTABLE_NAME SampleConfiguration)
add_executable(${EXECUTABLE_NAME} ${EXECUTABLE_SOURCES})
target_link_libraries(${EXECUTABLE_NAME} PUBLIC ${MODULE_NAME})
#=========================================================

#=========================================================

if(BUILD_Tests)
    add_subdirectory(test)
endif()
#=========================================================
