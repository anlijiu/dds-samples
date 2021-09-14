
idlcxx_generate(TARGET hvaclibrary
    FILES "${CMAKE_CURRENT_SOURCE_DIR}/msg/Ac.idl"
          "${CMAKE_CURRENT_SOURCE_DIR}/srv/SetPower.idl"
          "${CMAKE_CURRENT_SOURCE_DIR}/srv/SetRecirc.idl"
          "${CMAKE_CURRENT_SOURCE_DIR}/srv/SetFanSpeed.idl"
          "${CMAKE_CURRENT_SOURCE_DIR}/srv/SetFanDirection.idl"
          "${CMAKE_CURRENT_SOURCE_DIR}/srv/SetAc.idl"
          "${CMAKE_CURRENT_SOURCE_DIR}/srv/SetPtc.idl"
          "${CMAKE_CURRENT_SOURCE_DIR}/srv/SetTemperature.idl"
)

add_dependencies(hvaclibrary 
    stdlibrary 
)
message("hvac idl generated")
