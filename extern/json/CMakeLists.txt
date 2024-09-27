# ini library
message(STATUS "###########json###########")
set(LIBRARY_NAME "json")

set(INCLUDE_DIRS  ${PROJECT_SOURCE_DIR}/extern/json/include/)

add_library(${LIBRARY_NAME} INTERFACE)
target_include_directories(${LIBRARY_NAME} INTERFACE ${INCLUDE_DIRS})
target_compile_features(${LIBRARY_NAME} INTERFACE cxx_std_17)
