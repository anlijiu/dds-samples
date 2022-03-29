cmake_minimum_required(VERSION 3.22)
project(stdmsgslibrary LANGUAGES C CXX VERSION 1.0)

include(GNUInstallDirs)

find_package(CycloneDDS-CXX REQUIRED)
find_package(CycloneDDS REQUIRED)

include("../Generate.cmake")

cmake_path(GET CMAKE_CURRENT_SOURCE_DIR PARENT_PATH MY_PARENT_DIR)

idlcxx_generate(IDLCXX_SRCS IDLCXX_HDRS TARGET stdmsgdatatypes
    INCLUDES ${MY_PARENT_DIR}
    FILES "${CMAKE_CURRENT_SOURCE_DIR}/msg/ColorRGBA.idl"
          "${CMAKE_CURRENT_SOURCE_DIR}/msg/Duration.idl"
          "${CMAKE_CURRENT_SOURCE_DIR}/msg/Time.idl"
          "${CMAKE_CURRENT_SOURCE_DIR}/msg/Header.idl"
          # "${CMAKE_CURRENT_SOURCE_DIR}/msg/VehicleArea.idl"
          "${CMAKE_CURRENT_SOURCE_DIR}/srv/Response.idl"
)

configure_file(DataTypesStdConfig.cmake.in DataTypesStdConfig.cmake @ONLY)

add_library(stdmsgslibrary SHARED ${IDLCXX_SRCS})
target_link_libraries(stdmsgslibrary CycloneDDS-CXX::ddscxx)

# 写入库的版本信息
include(CMakePackageConfigHelpers)                                                        
write_basic_package_version_file(
    DataTypesStdConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion
    )   

install(FILES ${IDLCXX_SRCS} ${IDLCXX_HDRS} DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/std_msgs")

install(TARGETS stdmsgslibrary
    EXPORT StdMsgsTargets
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include
    )

# 将库的相关信息输出为DataTypesStdConfig.cmake，供find_package查找
install(EXPORT StdMsgsTargets
        FILE StdMsgsTargets.cmake
        NAMESPACE StdMsgs::
        DESTINATION lib/cmake/StdMsgs
        )

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/DataTypesStdConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/DataTypesStdConfigVersion.cmake"
    DESTINATION lib/cmake/StdMsgs
    )
