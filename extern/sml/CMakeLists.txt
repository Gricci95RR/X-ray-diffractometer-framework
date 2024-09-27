# ini library
message(STATUS "###########sml###########")
set(LIBRARY_NAME "sml")

set(INCLUDE_DIRS  ${PROJECT_SOURCE_DIR}/extern/sml/include/)

add_library(${LIBRARY_NAME} INTERFACE)
target_include_directories(${LIBRARY_NAME} INTERFACE ${INCLUDE_DIRS})
