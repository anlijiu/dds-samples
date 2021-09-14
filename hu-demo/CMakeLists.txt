cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(hu-demo VERSION 1.0.0)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/cmake")


set(api "cpp2")
set(cxx_standard CXX_STANDARD 11)

if(NOT RTIConnextDDS_FOUND)
    find_package(RTIConnextDDS
        "6.1.0"
        REQUIRED
        COMPONENTS
            core
            messaging_api
    )
endif()
find_package(soa-types)
# find_library(soa-types NAMES SoaDataTypes)

include_directories(
    "src/" 
    "src/hvac" 
    ${CONNEXTDDS_INCLUDE_DIRS}
)

add_executable(hu-demo
    src/vhal_compatible.cpp
    src/main.cpp
    src/AbsModule.cpp
    src/DDSMessageAdapter.cpp
    src/hvac/HvacModule.cpp
)

target_link_libraries(hu-demo
    PRIVATE
    soa-types::SoaDataTypes
    ${_CONNEXT_DEPENDENCIES}
    RTIConnextDDS::${api}_api
    RTIConnextDDS::messaging_${api}_api
)
# target_link_libraries(hu-demo
#     ${CONNEXTDDS_${lang_var}_API_LIBRARIES_RELEASE_SHARED}
#     ${CONNEXTDDS_EXTERNAL_LIBS} 
# )

target_include_directories(hu-demo PUBLIC 
    ${CONNEXTDDS_INCLUDE_DIRS} 
    $<TARGET_PROPERTY:RTIConnextDDS::${api}_api,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:RTIConnextDDS::messaging_${api}_api,INTERFACE_INCLUDE_DIRECTORIES>
)
    # $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/>

target_compile_definitions(hu-demo PRIVATE 
    $<TARGET_PROPERTY:RTIConnextDDS::${api}_api,INTERFACE_COMPILE_DEFINITIONS>
    $<TARGET_PROPERTY:RTIConnextDDS::messaging_${api}_api,INTERFACE_COMPILE_DEFINITIONS>
)
    # ${CONNEXTDDS_COMPILE_DEFINITIONS}


# Install target
install(TARGETS hu-demo
    EXPORT "${TARGETS_EXPORT_NAME}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
)
