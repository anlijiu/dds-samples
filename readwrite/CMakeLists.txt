project(readwrite LANGUAGES C)
cmake_minimum_required(VERSION 3.5)

if(NOT TARGET CycloneDDS::ddsc)
  find_package(CycloneDDS REQUIRED)
endif()

idlc_generate(TARGET ReadWriteData_lib FILES "ReadWriteData.idl")

add_executable(reader reader.c)
add_executable(writer writer.c)

target_link_libraries(reader ReadWriteData_lib CycloneDDS::ddsc)
target_link_libraries(writer ReadWriteData_lib CycloneDDS::ddsc)
