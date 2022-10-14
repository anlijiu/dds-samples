cmake_minimum_required(VERSION 3.22)
project(drivelibrary LANGUAGES C CXX VERSION 1.0)

include(GNUInstallDirs)

# find_package(CycloneDDS-CXX REQUIRED)
# find_package(CycloneDDS REQUIRED)

include("../Generate.cmake")
# include("${CycloneDDS-CXX}/idlcxx/Generate.cmake")
 
idlcxx_generate(IDLCXX_SRCS IDLCXX_HDRS TARGET drivedatatypes
# idlcxx_generate(TARGET drivedatatypes
  FILES "${CMAKE_CURRENT_SOURCE_DIR}/srv/AppointmentDrive.idl"
)
# add_custom_target(IDLCXX_SRCS DEPENDS drivedatatypes)

message("IDLCXX_SRCS:  ${IDLCXX_SRCS}  ")
message("IDLCXX_HDRS:  ${IDLCXX_HDRS}  ")
# add_dependencies(drivelibrary 
#     stdlibrary 
# )

configure_file(DataTypesDriveConfig.cmake.in DataTypesDriveConfig.cmake @ONLY)

add_library(drivelibrary SHARED ${IDLCXX_SRCS})
# add_dependencies(drivelibrary ${IDLCXX_TARGET})
# link_libraries(drivelibrary CycloneDDS-CXX::ddscxx drivedatatypes)
target_link_libraries(drivelibrary CycloneDDS-CXX::ddscxx)

# 写入库的版本信息
include(CMakePackageConfigHelpers)                                                        
write_basic_package_version_file(
    DataTypesDriveConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion
    )   

# install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/msg DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/drive_msgs")
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/srv DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/drive_msgs")

# install(FILES ${IDLCXX_SRCS} ${IDLCXX_HDRS} DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/drive_msgs")

# install(TARGETS drivedatatypes
#         EXPORT DataTypesDriveTargets
#         LIBRARY DESTINATION lib
#         PUBLIC_HEADER DESTINATION include
#         )

install(TARGETS drivelibrary
        EXPORT DataTypesDriveTargets
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include
        )

# 将库的相关信息输出为DataTypesDriveConfig.cmake，供find_package查找
install(EXPORT DataTypesDriveTargets
        FILE DataTypesDriveTargets.cmake
        NAMESPACE DataTypesDrive::
        DESTINATION lib/cmake/DataTypesDrive
        )

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/DataTypesDriveConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/DataTypesDriveConfigVersion.cmake"
        DESTINATION lib/cmake/DataTypesDrive
        )
