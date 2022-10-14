cmake_minimum_required(VERSION 3.22)
project(windowlibrary LANGUAGES C CXX VERSION 1.0)

include(GNUInstallDirs)

# find_package(CycloneDDS-CXX REQUIRED)
# find_package(CycloneDDS REQUIRED)

include("../Generate.cmake")
# include("${CycloneDDS-CXX}/idlcxx/Generate.cmake")
 
idlcxx_generate(IDLCXX_SRCS IDLCXX_HDRS TARGET windowdatatypes
# idlcxx_generate(TARGET windowdatatypes
    FILES "${CMAKE_CURRENT_SOURCE_DIR}/msg/Position.idl"
          "${CMAKE_CURRENT_SOURCE_DIR}/srv/SetPosition.idl"
)
# add_custom_target(IDLCXX_SRCS DEPENDS windowdatatypes)

message("IDLCXX_SRCS:  ${IDLCXX_SRCS}  ")
message("IDLCXX_HDRS:  ${IDLCXX_HDRS}  ")
# add_dependencies(windowlibrary 
#     stdlibrary 
# )

configure_file(DataTypesWindowConfig.cmake.in DataTypesWindowConfig.cmake @ONLY)

add_library(windowlibrary SHARED ${IDLCXX_SRCS})
# add_dependencies(windowlibrary ${IDLCXX_TARGET})
# link_libraries(windowlibrary CycloneDDS-CXX::ddscxx windowdatatypes)
target_link_libraries(windowlibrary CycloneDDS-CXX::ddscxx)

# 写入库的版本信息
include(CMakePackageConfigHelpers)                                                        
write_basic_package_version_file(
    DataTypesWindowConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion
    )   

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/msg DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/window_msgs")
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/srv DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/window_msgs")

# install(FILES ${IDLCXX_SRCS} ${IDLCXX_HDRS} DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/window_msgs")

# install(TARGETS windowdatatypes
#         EXPORT DataTypesWindowTargets
#         LIBRARY DESTINATION lib
#         PUBLIC_HEADER DESTINATION include
#         )

install(TARGETS windowlibrary
        EXPORT DataTypesWindowTargets
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include
        )

# 将库的相关信息输出为DataTypesWindowConfig.cmake，供find_package查找
install(EXPORT DataTypesWindowTargets
        FILE DataTypesWindowTargets.cmake
        NAMESPACE DataTypesWindow::
        DESTINATION lib/cmake/DataTypesWindow
        )

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/DataTypesWindowConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/DataTypesWindowConfigVersion.cmake"
        DESTINATION lib/cmake/DataTypesWindow
        )
