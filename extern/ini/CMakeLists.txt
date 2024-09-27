# ini library
message(STATUS "###########ini###########")
set(LIBRARY_NAME "ini")

set(INCLUDE_DIRS  ${PROJECT_SOURCE_DIR}/extern/ini/include/)

add_library(${LIBRARY_NAME} INTERFACE)
target_include_directories(${LIBRARY_NAME} INTERFACE ${INCLUDE_DIRS})
target_compile_features(${LIBRARY_NAME} INTERFACE cxx_std_17)
