# FindDP5.cmake

# DP5 Library - amptek::dp5

message("SETTING UP DP5 COMMUNCIATION LIBRARY FOR DP5")

set(libusb_REPOSITORY_TAG "v1.0.27")
message(STATUS "Setting up libusb repository version: ${libusb_REPOSITORY_TAG}")

FetchContent_Declare(
    LibusbCmake
    GIT_REPOSITORY https://github.com/libusb/libusb-cmake.git #(23/05/2024)
    GIT_TAG ${libusb_REPOSITORY_TAG}
)

FetchContent_MakeAvailable(LibusbCmake)
####

set(DP5_AMPTEK_PATH "${PROJECT_SOURCE_DIR}/extern/DP5_Amptek")
set(CONSOLE_HELPER_PATH "${DP5_AMPTEK_PATH}/ConsoleHelper")

set(LIB_DP5_MODULE_NAME "DP5_MODULE")
set(CONSOLE_HELPER_LIB_NAME "DP5_CONSOLE_MODULE")

set(dp5_SRC ${DP5_AMPTEK_PATH}/AsciiCmdUtilities.cpp
            ${DP5_AMPTEK_PATH}/DP5Protocol.cpp
            ${DP5_AMPTEK_PATH}/DP5Status.cpp
            ${DP5_AMPTEK_PATH}/DppLibUsb.cpp
            ${DP5_AMPTEK_PATH}/DppUtilities.cpp
            ${DP5_AMPTEK_PATH}/ParsePacket.cpp
            ${DP5_AMPTEK_PATH}/SendCommand.cpp
            ${DP5_AMPTEK_PATH}/stringex.cpp
            ${DP5_AMPTEK_PATH}/AsciiCmdUtilities.h
            ${DP5_AMPTEK_PATH}/DP5Protocol.h
            ${DP5_AMPTEK_PATH}/DP5Status.h
            ${DP5_AMPTEK_PATH}/DppConst.h
            ${DP5_AMPTEK_PATH}/DppLibUsb.h
            ${DP5_AMPTEK_PATH}/DppUtilities.h
            ${DP5_AMPTEK_PATH}/ParsePacket.h
            ${DP5_AMPTEK_PATH}/SendCommand.h
            ${DP5_AMPTEK_PATH}/stringex.h
            ${DP5_AMPTEK_PATH}/stringSplit.h
)

add_library(${LIB_DP5_MODULE_NAME} ${dp5_SRC})
target_link_libraries(${LIB_DP5_MODULE_NAME} PUBLIC usb-1.0)
target_include_directories(${LIB_DP5_MODULE_NAME} PUBLIC ${DP5_AMPTEK_PATH})
add_library(amptek::dp5 ALIAS ${LIB_DP5_MODULE_NAME})

message("Setting up console helper module")

set(consoleHelper_SRC ${CONSOLE_HELPER_PATH}/ConsoleHelper.cpp
                      ${CONSOLE_HELPER_PATH}/ConsoleHelper.h
)

add_library(${CONSOLE_HELPER_LIB_NAME} ${consoleHelper_SRC})
target_link_libraries(${CONSOLE_HELPER_LIB_NAME} PUBLIC amptek::dp5)
target_include_directories(${CONSOLE_HELPER_LIB_NAME} PUBLIC ${CONSOLE_HELPER_PATH})
add_library(amptek::console_helper ALIAS ${CONSOLE_HELPER_LIB_NAME})

message("=== DP5 Module set up finsihed ===")


