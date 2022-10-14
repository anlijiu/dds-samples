cmake_minimum_required(VERSION 3.22)
project(hvaclibrary LANGUAGES C CXX VERSION 1.0)

include(GNUInstallDirs)

# find_package(CycloneDDS-CXX REQUIRED)
# find_package(CycloneDDS REQUIRED)

include("../Generate.cmake")
# include("${CycloneDDS-CXX}/idlcxx/Generate.cmake")
 
idlcxx_generate(IDLCXX_SRCS IDLCXX_HDRS TARGET hvacdatatypes
# idlcxx_generate(TARGET hvacdatatypes
    FILES "${CMAKE_CURRENT_SOURCE_DIR}/msg/Ac.idl"
          "${CMAKE_CURRENT_SOURCE_DIR}/msg/Hvac.idl"
          "${CMAKE_CURRENT_SOURCE_DIR}/srv/HvacSrv.idl"
          "${CMAKE_CURRENT_SOURCE_DIR}/srv/SetPower.idl"
          "${CMAKE_CURRENT_SOURCE_DIR}/srv/SetRecirc.idl"
          "${CMAKE_CURRENT_SOURCE_DIR}/srv/SetFanSpeed.idl"
          "${CMAKE_CURRENT_SOURCE_DIR}/srv/SetFanDirection.idl"
          "${CMAKE_CURRENT_SOURCE_DIR}/srv/SetAc.idl"
          "${CMAKE_CURRENT_SOURCE_DIR}/srv/SetPtc.idl"
          "${CMAKE_CURRENT_SOURCE_DIR}/srv/SetTemperature.idl"
)
# add_custom_target(IDLCXX_SRCS DEPENDS hvacdatatypes)

message("IDLCXX_SRCS:  ${IDLCXX_SRCS}  ")
message("IDLCXX_HDRS:  ${IDLCXX_HDRS}  ")
# add_dependencies(hvaclibrary 
#     stdlibrary 
# )

configure_file(DataTypesHvacConfig.cmake.in DataTypesHvacConfig.cmake @ONLY)

add_library(hvaclibrary SHARED ${IDLCXX_SRCS})
# add_dependencies(hvaclibrary ${IDLCXX_TARGET})
# link_libraries(hvaclibrary CycloneDDS-CXX::ddscxx hvacdatatypes)
target_link_libraries(hvaclibrary CycloneDDS-CXX::ddscxx)

# 写入库的版本信息
include(CMakePackageConfigHelpers)                                                        
write_basic_package_version_file(
    DataTypesHvacConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion
    )   

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/msg DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/hvac_msgs")
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/srv DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/hvac_msgs")

# install(FILES ${IDLCXX_SRCS} ${IDLCXX_HDRS} DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/hvac_msgs")

# install(TARGETS hvacdatatypes
#         EXPORT DataTypesHvacTargets
#         LIBRARY DESTINATION lib
#         PUBLIC_HEADER DESTINATION include
#         )

install(TARGETS hvaclibrary
        EXPORT DataTypesHvacTargets
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include
        )

# 将库的相关信息输出为DataTypesHvacConfig.cmake，供find_package查找
install(EXPORT DataTypesHvacTargets
        FILE DataTypesHvacTargets.cmake
        NAMESPACE DataTypesHvac::
        DESTINATION lib/cmake/DataTypesHvac
        )

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/DataTypesHvacConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/DataTypesHvacConfigVersion.cmake"
        DESTINATION lib/cmake/DataTypesHvac
        )
