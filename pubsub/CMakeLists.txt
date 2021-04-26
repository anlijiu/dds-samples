project(throughput LANGUAGES C)
cmake_minimum_required(VERSION 3.5)

if(NOT TARGET CycloneDDS::ddsc)
  find_package(CycloneDDS REQUIRED)
endif()

idlc_generate(TARGET PubSub_lib FILES PubSub.idl)

add_executable(publisher publisher.c)
add_executable(subscriber subscriber.c)

target_link_libraries(publisher PubSub_lib CycloneDDS::ddsc)
target_link_libraries(subscriber PubSub_lib CycloneDDS::ddsc)
