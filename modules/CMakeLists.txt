if(BUILD_Communication)
    add_subdirectory(Communication)
endif()

if(BUILD_ConfigurationFileParser)
    add_subdirectory(ConfigurationFileParser)
endif()

if(BUILD_Devices)
    add_subdirectory(Devices)
endif()

if(BUILD_Motors)
    add_subdirectory(Motors)
endif()

if(BUILD_PostProcessing)
    add_subdirectory(PostProcessing)
endif()

if(BUILD_Scan)
    add_subdirectory(Scan)
endif()

if(BUILD_Sensors)
    add_subdirectory(Sensors)
endif()

if(BUILD_Template)
    add_subdirectory(Template)
endif()

if(BUILD_UIManagementServer)
    add_subdirectory(UIManagementServer)
endif()

if(BUILD_XRaySensor)
    add_subdirectory(XRaySensor)
endif()
