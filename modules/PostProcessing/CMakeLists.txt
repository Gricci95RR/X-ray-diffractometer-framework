message(STATUS "###########PostProcessing###########")
set(MODULE_NAME "PostProcessing")

set(MODULE_LIBS spdlog::spdlog
                gtest_main
                gmock
)

set(INCLUDE_DIRS ./include
)

set(SRC_FILES ./src/PostProcessing.cpp
              ./include/PostProcessingMock.hpp
              ./include/PostProcessingMockConfiguration.hpp
)

add_library(${MODULE_NAME} ${SRC_FILES})

target_include_directories(${MODULE_NAME} PUBLIC ${INCLUDE_DIRS})
target_link_libraries(${MODULE_NAME} PUBLIC ${MODULE_LIBS})

#=========================================================s
# Executable file (Samples)
set(EXECUTABLE_SOURCES ${PROJECT_SOURCE_DIR}/modules/PostProcessing/samples/PostProcessing_Sample.cpp)
set(EXECUTABLE_NAME PostProcessing_Sample)
add_executable(${EXECUTABLE_NAME} ${EXECUTABLE_SOURCES})
target_link_libraries(${EXECUTABLE_NAME} PUBLIC ${MODULE_NAME})

#=========================================================

#=========================================================

#=========================================================
