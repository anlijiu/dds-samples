include(ConnextDdsSoaDdsTypes)

list(APPEND idl_files "${CMAKE_CURRENT_SOURCE_DIR}/msg/Ac.idl")

list(APPEND idl_files "${CMAKE_CURRENT_SOURCE_DIR}/srv/SetPower.idl")
list(APPEND idl_files "${CMAKE_CURRENT_SOURCE_DIR}/srv/SetRecirc.idl")
list(APPEND idl_files "${CMAKE_CURRENT_SOURCE_DIR}/srv/SetFanSpeed.idl")
list(APPEND idl_files "${CMAKE_CURRENT_SOURCE_DIR}/srv/SetFanDirection.idl")
list(APPEND idl_files "${CMAKE_CURRENT_SOURCE_DIR}/srv/SetAc.idl")
list(APPEND idl_files "${CMAKE_CURRENT_SOURCE_DIR}/srv/SetPtc.idl")
list(APPEND idl_files "${CMAKE_CURRENT_SOURCE_DIR}/srv/SetTemperature.idl")

connextdds_generate_soa_dds_types(
    LANG ${LANG}
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    IDL_FILES ${idl_files}
    INCLUDE_DIRS ${top_level_source_dir}
)

add_library( hvaclibrary OBJECT
    ${generated_file_list}
)

set_property(TARGET hvaclibrary PROPERTY 
    POSITION_INDEPENDENT_CODE ON
)

target_include_directories(hvaclibrary PRIVATE 
    ${CONNEXTDDS_INCLUDE_DIRS}
    ${top_level_binary_dir}
)

target_compile_definitions(hvaclibrary PRIVATE 
    ${CONNEXTDDS_COMPILE_DEFINITIONS}
)

add_dependencies(hvaclibrary 
    stdlibrary 
)
