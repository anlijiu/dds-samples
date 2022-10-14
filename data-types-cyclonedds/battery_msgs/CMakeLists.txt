cmake_minimum_required(VERSION 3.22)
project(batterylibrary LANGUAGES C CXX VERSION 1.0)

include(GNUInstallDirs)

find_package(CycloneDDS-CXX REQUIRED)
find_package(CycloneDDS REQUIRED)

include("../Generate.cmake")
# include("${CycloneDDS-CXX}/idlcxx/Generate.cmake")
 
idlcxx_generate(IDLCXX_SRCS IDLCXX_HDRS TARGET batterydatatypes
# idlcxx_generate(TARGET batterydatatypes
    FILES "${CMAKE_CURRENT_SOURCE_DIR}/srv/Prepare.idl"
)
# add_custom_target(IDLCXX_SRCS DEPENDS batterydatatypes)

message("IDLCXX_SRCS:  ${IDLCXX_SRCS}  ")
message("IDLCXX_HDRS:  ${IDLCXX_HDRS}  ")
# add_dependencies(batterylibrary 
#     stdlibrary 
# )

configure_file(DataTypesBatteryConfig.cmake.in DataTypesBatteryConfig.cmake @ONLY)

add_library(batterylibrary SHARED ${IDLCXX_SRCS})
# add_dependencies(batterylibrary ${IDLCXX_TARGET})
# link_libraries(batterylibrary CycloneDDS-CXX::ddscxx batterydatatypes)
target_link_libraries(batterylibrary CycloneDDS-CXX::ddscxx)

# 写入库的版本信息
include(CMakePackageConfigHelpers)                                                        
write_basic_package_version_file(
    DataTypesBatteryConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion
    )   

# install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/msg DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/battery_msgs")
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/srv DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/battery_msgs")

# install(FILES ${IDLCXX_SRCS} ${IDLCXX_HDRS} DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/battery_msgs")

# install(TARGETS batterydatatypes
#         EXPORT DataTypesBatteryTargets
#         LIBRARY DESTINATION lib
#         PUBLIC_HEADER DESTINATION include
#         )

install(TARGETS batterylibrary
        EXPORT DataTypesBatteryTargets
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include
        )

# 将库的相关信息输出为DataTypesBatteryConfig.cmake，供find_package查找
install(EXPORT DataTypesBatteryTargets
        FILE DataTypesBatteryTargets.cmake
        NAMESPACE DataTypesBattery::
        DESTINATION lib/cmake/DataTypesBattery
        )

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/DataTypesBatteryConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/DataTypesBatteryConfigVersion.cmake"
        DESTINATION lib/cmake/DataTypesBattery
        )
