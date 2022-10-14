cmake_minimum_required(VERSION 3.22)
project(habitlibrary LANGUAGES C CXX VERSION 1.0)

include(GNUInstallDirs)

# find_package(CycloneDDS-CXX REQUIRED)
# find_package(CycloneDDS REQUIRED)

include("../Generate.cmake")
# include("${CycloneDDS-CXX}/idlcxx/Generate.cmake")
 
idlcxx_generate(IDLCXX_SRCS IDLCXX_HDRS TARGET habitdatatypes
    # idlcxx_generate(TARGET habitdatatypes
    FILES "${CMAKE_CURRENT_SOURCE_DIR}/srv/AppointmentDrive.idl"
)
# add_custom_target(IDLCXX_SRCS DEPENDS habitdatatypes)

message("IDLCXX_SRCS:  ${IDLCXX_SRCS}  ")
message("IDLCXX_HDRS:  ${IDLCXX_HDRS}  ")
# add_dependencies(habitlibrary 
#     stdlibrary 
# )

configure_file(DataTypesHabitConfig.cmake.in DataTypesHabitConfig.cmake @ONLY)

add_library(habitlibrary SHARED ${IDLCXX_SRCS})
# add_dependencies(habitlibrary ${IDLCXX_TARGET})
# link_libraries(habitlibrary CycloneDDS-CXX::ddscxx habitdatatypes)
target_link_libraries(habitlibrary CycloneDDS-CXX::ddscxx)

# 写入库的版本信息
include(CMakePackageConfigHelpers)                                                        
write_basic_package_version_file(
    DataTypesHabitConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion
    )   

# install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/msg DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/habit_msgs")
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/srv DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/habit_msgs")

# install(FILES ${IDLCXX_SRCS} ${IDLCXX_HDRS} DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/habit_msgs")

# install(TARGETS habitdatatypes
#         EXPORT DataTypesHabitTargets
#         LIBRARY DESTINATION lib
#         PUBLIC_HEADER DESTINATION include
#         )

install(TARGETS habitlibrary
        EXPORT DataTypesHabitTargets
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include
        )

# 将库的相关信息输出为DataTypesHabitConfig.cmake，供find_package查找
install(EXPORT DataTypesHabitTargets
        FILE DataTypesHabitTargets.cmake
        NAMESPACE DataTypesHabit::
        DESTINATION lib/cmake/DataTypesHabit
        )

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/DataTypesHabitConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/DataTypesHabitConfigVersion.cmake"
        DESTINATION lib/cmake/DataTypesHabit
        )
