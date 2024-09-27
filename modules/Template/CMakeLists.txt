set(MODULE_NAME "Template")

find_package(HXP REQUIRED)
find_package(XIMC REQUIRED)
find_package(DP5 REQUIRED) #DP5 XRAY SENSOR IMPLEMENTATION

set(MODULE_LIBS HXP_LIB
                XIMC_LIB
)

set(INCLUDE_DIRS ./include
)

set(SRC_FILES ./src/Hello.cpp
)

add_library(${MODULE_NAME} ${SRC_FILES})

target_include_directories(${MODULE_NAME} PUBLIC ${INCLUDE_DIRS})
target_link_libraries(${MODULE_NAME} PRIVATE ${MODULE_LIBS})

#=========================================================s
# Hello Example
add_executable(HelloSample ./samples/HelloSample.cpp)
target_link_libraries(HelloSample ${MODULE_NAME} spdlog::spdlog amptek::console_helper)

#=========================================================
#Setup shared libraries in post-build
setup_dll_postbuild(TARGET HelloSample)

#Debugging expression
#file(GENERATE OUTPUT "C:/dev/CPP/XRay_Machine/build/output_log.txt" CONTENT "$<TARGET_RUNTIME_DLLS:HelloSample>")
#=========================================================

if(BUILD_Tests)
    add_subdirectory(test)
endif()
#=========================================================