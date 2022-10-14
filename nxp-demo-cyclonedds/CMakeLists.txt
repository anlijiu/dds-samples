project(nxp LANGUAGES C CXX)
cmake_minimum_required(VERSION 3.5)

if(NOT TARGET CycloneDDS-CXX::ddscxx)
  find_package(CycloneDDS-CXX REQUIRED)
endif()

# message("CMAKE_PREFIX_PATH:  ${CMAKE_PREFIX_PATH}")
# message("CMAKE_FRAMEWORK_PATH:  ${CMAKE_FRAMEWORK_PATH}")
# message("CMAKE_APPBUNDLE_PATH:  ${CMAKE_APPBUNDLE_PATH}")


find_package(fmt REQUIRED)
find_package(DataTypesCloud REQUIRED)
find_package(DataTypesHvac REQUIRED)
find_package(DataTypesDrive REQUIRED)
find_package(DataTypesBattery REQUIRED)
find_package(DataTypesCabin REQUIRED)
find_package(DataTypesHabit REQUIRED)
find_package(DataTypesSeat REQUIRED)
find_package(DataTypesWindow REQUIRED)
find_package(DataTypesStd REQUIRED)

include_directories(
    "src/" 
)

add_executable(nxp
    src/libwecan.c
    src/vhal_compatible.cpp
    src/Log.cpp
    src/DdsUtils.cpp
    src/main.cpp
    src/DataHub.cpp
    src/AbsModule.cpp
    src/CanBusMessageAdapter.cpp
    src/CanTranseiver.cpp
    src/CanTransformer.cpp
    src/DdsMessageAdapter.cpp
    src/DdsTranseiver.cpp
    src/ObjectPool.cpp
    src/VehiclePropertyStore.cpp
    src/NxpService.cpp
    src/AsyncWaitSet.cpp
)

# target_link_libraries(hu-demo CycloneDDS-CXX::ddscxx hvaclibrary stdmsgslibrary)
target_link_libraries(nxp fmt::fmt CycloneDDS-CXX::ddscxx DataTypesCloud::cloudlibrary DataTypesCabin::cabinlibrary DataTypesHvac::hvaclibrary DataTypesSeat::seatlibrary DataTypesBattery::batterylibrary DataTypesHabit::habitlibrary DataTypesStd::stdlibrary)

# Disable the static analyzer in GCC to avoid crashing the GNU C++ compiler
# on Azure Pipelines
if(DEFINED ENV{SYSTEM_TEAMFOUNDATIONSERVERURI})
  if(CMAKE_C_COMPILER_ID STREQUAL "GNU" AND ANALYZER STREQUAL "on")
    target_compile_options(nxp PRIVATE -fno-analyzer)
    target_compile_options(nxp PRIVATE -fno-analyzer)
  endif()
endif()

set_property(TARGET nxp PROPERTY CXX_STANDARD 17)
