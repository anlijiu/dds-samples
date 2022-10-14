project(hu LANGUAGES C CXX)
cmake_minimum_required(VERSION 3.5)

if(NOT TARGET CycloneDDS-CXX::ddscxx)
  find_package(CycloneDDS-CXX REQUIRED)
endif()


include_directories(
    "src/" 
    "src/hvac" 
    "src/seat" 
    "src/habit" 
)

add_executable(hu-demo
    src/vhal_compatible.cpp
    src/DdsUtils.cpp
    src/main.cpp
    src/AbsModule.cpp
    src/DDSMessageAdapter.cpp
    src/AsyncWaitSet.cpp
    src/hvac/HvacModule.cpp
    src/seat/SeatModule.cpp
    src/habit/HabitModule.cpp
)

# target_link_libraries(hu-demo CycloneDDS-CXX::ddscxx hvaclibrary stdmsgslibrary)
target_link_libraries(hu-demo CycloneDDS-CXX::ddscxx hvaclibrary seatlibrary habitlibrary stdmsgslibrary)

# Disable the static analyzer in GCC to avoid crashing the GNU C++ compiler
# on Azure Pipelines
if(DEFINED ENV{SYSTEM_TEAMFOUNDATIONSERVERURI})
  if(CMAKE_C_COMPILER_ID STREQUAL "GNU" AND ANALYZER STREQUAL "on")
    target_compile_options(hu-demo PRIVATE -fno-analyzer)
    target_compile_options(hu-demo PRIVATE -fno-analyzer)
  endif()
endif()

set_property(TARGET hu-demo PROPERTY CXX_STANDARD 17)
