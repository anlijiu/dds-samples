
idlcxx_generate(
    TARGET stdlibrary
    FILES "${CMAKE_CURRENT_SOURCE_DIR}/msg/ColorRGBA.idl"
          "${CMAKE_CURRENT_SOURCE_DIR}/msg/Duration.idl"
          "${CMAKE_CURRENT_SOURCE_DIR}/msg/Header.idl"
          "${CMAKE_CURRENT_SOURCE_DIR}/msg/Time.idl"
          # "${CMAKE_CURRENT_SOURCE_DIR}/msg/VehicleArea.idl"
          "${CMAKE_CURRENT_SOURCE_DIR}/srv/Response.idl"
)

