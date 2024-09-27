set(MODULE_NAME "Motors")

find_package(HXP REQUIRED)
find_package(XIMC REQUIRED)

#set(MODULE_LIBS ${XIMC_LIBRARIES}
#)
#set(MODULE_LIBS2 ${HXP_LIBRARIES}
#)

set(MODULE_LIBS HXP_LIB
                XIMC_LIB
                ConfigurationFileParser
                gtest_main
                gmock
)

set(INCLUDE_DIRS ./include/HXP 
                 ./include/Steppers)

#set(INCLUDE_DIRS ./include
#                  ${XIMC_INCLUDE_DIRS} ${HXP_INCLUDE_DIRS}
#)

set(SRC_FILES ./src/HXP.cpp  ./src/XIMC.cpp 
              ./include/HXP/HXPMock.hpp ./include/HXP/HXPMockConfiguration.hpp 
              ./include/Steppers/StepperMock.hpp ./include/Steppers/StepperMockConfiguration.hpp 
)

#add_library(${MODULE_NAME} STATIC ${SRC_FILES})

add_library(${MODULE_NAME}  ${SRC_FILES})
target_include_directories(${MODULE_NAME} PUBLIC ${INCLUDE_DIRS})
target_link_libraries(${MODULE_NAME} PUBLIC ${MODULE_LIBS})

# Samples
add_executable(main ./samples/main.cpp)
target_link_libraries(main ${MODULE_NAME} ${MODULE_LIBS})

add_subdirectory(test)