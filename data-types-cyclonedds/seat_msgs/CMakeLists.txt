cmake_minimum_required(VERSION 3.22)
project(seatlibrary LANGUAGES C CXX VERSION 1.0)

include(GNUInstallDirs)

# find_package(CycloneDDS-CXX REQUIRED)
# find_package(CycloneDDS REQUIRED)

include("../Generate.cmake")
# include("${CycloneDDS-CXX}/idlcxx/Generate.cmake")
 
idlcxx_generate(IDLCXX_SRCS IDLCXX_HDRS TARGET seatdatatypes
# idlcxx_generate(TARGET seatdatatypes
    FILES "${CMAKE_CURRENT_SOURCE_DIR}/msg/Seat.idl"
          "${CMAKE_CURRENT_SOURCE_DIR}/srv/SeatSrv.idl"
)
# add_custom_target(IDLCXX_SRCS DEPENDS seatdatatypes)

message("IDLCXX_SRCS:  ${IDLCXX_SRCS}  ")
message("IDLCXX_HDRS:  ${IDLCXX_HDRS}  ")
# add_dependencies(seatlibrary 
#     stdlibrary 
# )

configure_file(DataTypesSeatConfig.cmake.in DataTypesSeatConfig.cmake @ONLY)

add_library(seatlibrary SHARED ${IDLCXX_SRCS})
# add_dependencies(seatlibrary ${IDLCXX_TARGET})
# link_libraries(seatlibrary CycloneDDS-CXX::ddscxx seatdatatypes)
target_link_libraries(seatlibrary CycloneDDS-CXX::ddscxx)

# 写入库的版本信息
include(CMakePackageConfigHelpers)                                                        
write_basic_package_version_file(
    DataTypesSeatConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion
    )   

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/msg DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/seat_msgs")
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/srv DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/seat_msgs")

# install(FILES ${IDLCXX_SRCS} ${IDLCXX_HDRS} DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/seat_msgs")

# install(TARGETS seatdatatypes
#         EXPORT DataTypesSeatTargets
#         LIBRARY DESTINATION lib
#         PUBLIC_HEADER DESTINATION include
#         )

install(TARGETS seatlibrary
        EXPORT DataTypesSeatTargets
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include
        )

# 将库的相关信息输出为DataTypesSeatConfig.cmake，供find_package查找
install(EXPORT DataTypesSeatTargets
        FILE DataTypesSeatTargets.cmake
        NAMESPACE DataTypesSeat::
        DESTINATION lib/cmake/DataTypesSeat
        )

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/DataTypesSeatConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/DataTypesSeatConfigVersion.cmake"
        DESTINATION lib/cmake/DataTypesSeat
        )
