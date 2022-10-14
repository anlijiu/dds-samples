cmake_minimum_required(VERSION 3.22)
project(cabinlibrary LANGUAGES C CXX VERSION 1.0)

include(GNUInstallDirs)

# find_package(CycloneDDS-CXX REQUIRED)
# find_package(CycloneDDS REQUIRED)

include("../Generate.cmake")
# include("${CycloneDDS-CXX}/idlcxx/Generate.cmake")
 
idlcxx_generate(IDLCXX_SRCS IDLCXX_HDRS TARGET cabindatatypes
# idlcxx_generate(TARGET cabindatatypes
    FILES "${CMAKE_CURRENT_SOURCE_DIR}/msg/Cabin.idl"
)
# add_custom_target(IDLCXX_SRCS DEPENDS cabindatatypes)

message("IDLCXX_SRCS:  ${IDLCXX_SRCS}  ")
message("IDLCXX_HDRS:  ${IDLCXX_HDRS}  ")
# add_dependencies(cabinlibrary 
#     stdlibrary 
# )

configure_file(DataTypesCabinConfig.cmake.in DataTypesCabinConfig.cmake @ONLY)

add_library(cabinlibrary SHARED ${IDLCXX_SRCS})
# add_dependencies(cabinlibrary ${IDLCXX_TARGET})
# link_libraries(cabinlibrary CycloneDDS-CXX::ddscxx cabindatatypes)
target_link_libraries(cabinlibrary CycloneDDS-CXX::ddscxx)

# 写入库的版本信息
include(CMakePackageConfigHelpers)                                                        
write_basic_package_version_file(
    DataTypesCabinConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion
    )   

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/msg DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/cabin_msgs")
# install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/srv DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/cabin_msgs")

# install(FILES ${IDLCXX_SRCS} ${IDLCXX_HDRS} DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/cabin_msgs")

# install(TARGETS cabindatatypes
#         EXPORT DataTypesCabinTargets
#         LIBRARY DESTINATION lib
#         PUBLIC_HEADER DESTINATION include
#         )

install(TARGETS cabinlibrary
        EXPORT DataTypesCabinTargets
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include
        )

# 将库的相关信息输出为DataTypesCabinConfig.cmake，供find_package查找
install(EXPORT DataTypesCabinTargets
        FILE DataTypesCabinTargets.cmake
        NAMESPACE DataTypesCabin::
        DESTINATION lib/cmake/DataTypesCabin
        )

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/DataTypesCabinConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/DataTypesCabinConfigVersion.cmake"
        DESTINATION lib/cmake/DataTypesCabin
        )
