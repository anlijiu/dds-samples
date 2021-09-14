cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(soa-types VERSION 1.0.0)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/cmake")

find_package(RTIConnextDDS)
include(CMakePackageConfigHelpers)
include(ConnextDdsSoaDdsTypes)

if (NOT LANG)
    set(LANG "C++11")
endif()

set (top_level_binary_dir "${CMAKE_CURRENT_BINARY_DIR}/")
set (top_level_source_dir "${CMAKE_CURRENT_SOURCE_DIR}/")

connextdds_sanitize_library_language(LANG ${LANG} VAR lang_var)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/std_msgs")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/hvac_msgs")

add_library(SoaDataTypes
    $<TARGET_OBJECTS:stdlibrary>
    $<TARGET_OBJECTS:hvaclibrary>
)

target_link_libraries(SoaDataTypes 
    ${CONNEXTDDS_${lang_var}_API_LIBRARIES_RELEASE_SHARED}
    ${CONNEXTDDS_EXTERNAL_LIBS} 
)

target_include_directories(SoaDataTypes PUBLIC 
    ${CONNEXTDDS_INCLUDE_DIRS} 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/>
)

target_compile_definitions(SoaDataTypes PRIVATE 
    ${CONNEXTDDS_COMPILE_DEFINITIONS}
)

# Set configuration variables
set(version_config 
    "${CMAKE_CURRENT_BINARY_DIR}/generated/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config 
    "${CMAKE_CURRENT_BINARY_DIR}/generated/${PROJECT_NAME}Config.cmake")
set(TARGETS_EXPORT_NAME "${PROJECT_NAME}Targets")

# Write configuration version file
write_basic_package_version_file(
    "${version_config}" COMPATIBILITY SameMajorVersion
)

# Create a config file of the library
configure_package_config_file(
    "resources/cmake/Config.cmake.in"
    "${project_config}"
    INSTALL_DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/${PROJECT_NAME}"
)

# Install target
install(TARGETS SoaDataTypes
    EXPORT "${TARGETS_EXPORT_NAME}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
    RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
    INCLUDES DESTINATION "${CMAKE_INSTALL_PREFIX}/include"
)

# Install config files
install(
    FILES "${project_config}" "${version_config}"
    DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/${PROJECT_NAME}"
)

install(
    EXPORT "${TARGETS_EXPORT_NAME}"
    NAMESPACE "${PROJECT_NAME}::"
    DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/${PROJECT_NAME}"
)
