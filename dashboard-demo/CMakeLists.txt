cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(dashboard-demo VERSION 1.0.0)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/cmake")

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -pg")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -pg")
SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -O3 -pg")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -O3 -pg")

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

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -latomic")

find_package(soa-types)
# find_library(soa-types NAMES SoaDataTypes)

include_directories(
    "src/" 
    "src/hvac" 
    ${CONNEXTDDS_INCLUDE_DIRS}
)

add_executable(${CMAKE_PROJECT_NAME}
    src/libwecan.c
    src/main.cpp
    src/DataHub.cpp
    src/CanMessageDistributor.cpp
    src/AbsModule.cpp
    src/hvac/HvacModule.cpp
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE
    soa-types::SoaDataTypes
    ${_CONNEXT_DEPENDENCIES}
    RTIConnextDDS::${api}_api
    RTIConnextDDS::messaging_${api}_api
)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC 
    ${CONNEXTDDS_INCLUDE_DIRS} 
    $<TARGET_PROPERTY:RTIConnextDDS::${api}_api,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:RTIConnextDDS::messaging_${api}_api,INTERFACE_INCLUDE_DIRECTORIES>
)
    # $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/>

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE 
    $<TARGET_PROPERTY:RTIConnextDDS::${api}_api,INTERFACE_COMPILE_DEFINITIONS>
    $<TARGET_PROPERTY:RTIConnextDDS::messaging_${api}_api,INTERFACE_COMPILE_DEFINITIONS>
)
    # ${CONNEXTDDS_COMPILE_DEFINITIONS}


# Install target
install(TARGETS ${CMAKE_PROJECT_NAME}
    EXPORT "${TARGETS_EXPORT_NAME}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
)
