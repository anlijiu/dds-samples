cmake_minimum_required(VERSION 3.22)
project(cloudlibrary LANGUAGES C CXX VERSION 1.0)

include(GNUInstallDirs)

# find_package(CycloneDDS-CXX REQUIRED)
# find_package(CycloneDDS REQUIRED)

include("../Generate.cmake")
# include("${CycloneDDS-CXX}/idlcxx/Generate.cmake")
 
idlcxx_generate(IDLCXX_SRCS IDLCXX_HDRS TARGET clouddatatypes
  FILES "${CMAKE_CURRENT_SOURCE_DIR}/msg/Cloud.idl"
)
# add_custom_target(IDLCXX_SRCS DEPENDS clouddatatypes)

message("IDLCXX_SRCS:  ${IDLCXX_SRCS}  ")
message("IDLCXX_HDRS:  ${IDLCXX_HDRS}  ")
# add_dependencies(cloudlibrary 
#     stdlibrary 
# )

configure_file(DataTypesCloudConfig.cmake.in DataTypesCloudConfig.cmake @ONLY)

add_library(cloudlibrary SHARED ${IDLCXX_SRCS})
# add_dependencies(cloudlibrary ${IDLCXX_TARGET})
# link_libraries(cloudlibrary CycloneDDS-CXX::ddscxx clouddatatypes)
target_link_libraries(cloudlibrary CycloneDDS-CXX::ddscxx)

# 写入库的版本信息
include(CMakePackageConfigHelpers)                                                        
write_basic_package_version_file(
    DataTypesCloudConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion
    )   

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/msg DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/cloud_msgs")
# install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/srv DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/cloud_msgs")

# install(FILES ${IDLCXX_SRCS} ${IDLCXX_HDRS} DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/cloud_msgs")

# install(TARGETS clouddatatypes
#         EXPORT DataTypesCloudTargets
#         LIBRARY DESTINATION lib
#         PUBLIC_HEADER DESTINATION include
#         )

install(TARGETS cloudlibrary
        EXPORT DataTypesCloudTargets
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include
        )

# 将库的相关信息输出为DataTypesCloudConfig.cmake，供find_package查找
install(EXPORT DataTypesCloudTargets
        FILE DataTypesCloudTargets.cmake
        NAMESPACE DataTypesCloud::
        DESTINATION lib/cmake/DataTypesCloud
        )

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/DataTypesCloudConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/DataTypesCloudConfigVersion.cmake"
        DESTINATION lib/cmake/DataTypesCloud
        )
