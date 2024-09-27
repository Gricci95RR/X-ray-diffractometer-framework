set(MODULE_NAME "Communication")

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

set(MODULE_LIBS gtest_main
                gmock
                asio::asio
                json
                spdlog::spdlog
                ConfigurationFileParser
)

set(INCLUDE_DIRS 
                  ./include
                  ./include/TCP
)

set(SRC_FILES 
              ./src/TCP/TCPSocket.cpp
              ./src/TCP/TCPServer.cpp
)

add_library(${MODULE_NAME} ${SRC_FILES})
target_include_directories(${MODULE_NAME} PUBLIC ${INCLUDE_DIRS})
target_link_libraries(${MODULE_NAME} PUBLIC ${MODULE_LIBS})

#=========================================================s
# Executable file (Samples)
set(EXECUTABLE_SOURCES ${PROJECT_SOURCE_DIR}/modules/Communication/samples/mainServer.cpp)
set(EXECUTABLE_NAME TCPServer)
add_executable(${EXECUTABLE_NAME} ${EXECUTABLE_SOURCES})
target_link_libraries(${EXECUTABLE_NAME} PUBLIC ${MODULE_NAME} ${MODULE_LIBS})

#=========================================================
