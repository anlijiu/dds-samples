include(ConnextDdsSoaDdsTypes)

list(APPEND idl_files "${CMAKE_CURRENT_SOURCE_DIR}/msg/ColorRGBA.idl")
list(APPEND idl_files "${CMAKE_CURRENT_SOURCE_DIR}/msg/Duration.idl")
list(APPEND idl_files "${CMAKE_CURRENT_SOURCE_DIR}/msg/Header.idl")
list(APPEND idl_files "${CMAKE_CURRENT_SOURCE_DIR}/msg/Time.idl")


connextdds_generate_soa_dds_types(
    LANG ${LANG}
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    IDL_FILES ${idl_files}
    INCLUDE_DIRS ${top_level_source_dir}
)

add_library( stdlibrary OBJECT
    ${generated_file_list}
)

set_property(TARGET stdlibrary PROPERTY 
    POSITION_INDEPENDENT_CODE ON
)

target_include_directories(stdlibrary PRIVATE 
    ${CONNEXTDDS_INCLUDE_DIRS}
    ${top_level_binary_dir}
)

target_compile_definitions(stdlibrary PRIVATE 
    ${CONNEXTDDS_COMPILE_DEFINITIONS}
)
