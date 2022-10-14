cmake_minimum_required(VERSION 3.5)

project(zcu-right-demo-cyclonedds VERSION 1.0.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)

if(NOT TARGET CycloneDDS-CXX::ddscxx)
  find_package(CycloneDDS-CXX REQUIRED)
endif()

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -latomic")

include_directories(
    "src/" 
    "src/seat" 
)

file(GLOB zcu-right_sources
    src/**.c
    src/**.cpp
    src/seat/**.cpp
)
add_executable(${CMAKE_PROJECT_NAME} ${zcu-right_sources})

target_link_libraries(${CMAKE_PROJECT_NAME} CycloneDDS-CXX::ddscxx atomic m seatlibrary hvaclibrary stdmsgslibrary)

# Disable the static analyzer in GCC to avoid crashing the GNU C++ compiler
# on Azure Pipelines
if(DEFINED ENV{SYSTEM_TEAMFOUNDATIONSERVERURI})
  if(CMAKE_C_COMPILER_ID STREQUAL "GNU" AND ANALYZER STREQUAL "on")
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -fno-analyzer)
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -fno-analyzer)
  endif()
endif()

set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 17)
target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE cxx_std_17)
