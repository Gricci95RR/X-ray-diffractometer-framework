set(MODULE_NAME "XRaySensor")

#=========================================================
# Download asio setup repository
include(FetchContent)
FetchContent_Declare(
  asiocmake
  GIT_REPOSITORY "https://github.com/OlivierLDff/asio.cmake"
  GIT_TAG        "main"
)
FetchContent_MakeAvailable(asiocmake)
#=========================================================

find_package(DP5 REQUIRED)

set(MODULE_LIBS ConfigurationFileParser
                spdlog::spdlog
                asio::asio 
                amptek::console_helper
)

set(INCLUDE_DIRS ./include)

set(SRC_FILES ./src/XRaySensor.cpp ./include/XRaySensorMock.hpp ./include/XRaySensorMockConfiguration.hpp
)

add_library(${MODULE_NAME} ${SRC_FILES})
target_include_directories(${MODULE_NAME} PUBLIC ${INCLUDE_DIRS})
target_link_libraries(${MODULE_NAME} PUBLIC ${MODULE_LIBS})

#=========================================================
add_executable(XRaySensorSample ./samples/XRaySensorSample.cpp)
target_include_directories(XRaySensorSample PRIVATE ${INCLUDE_DIRS})
target_link_libraries(XRaySensorSample PRIVATE ${MODULE_NAME})
#=========================================================

add_subdirectory(test)