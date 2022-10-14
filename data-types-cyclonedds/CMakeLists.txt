cmake_minimum_required (VERSION 3.5)

project(datatypes)

find_package(CycloneDDS-CXX REQUIRED)
find_package(CycloneDDS REQUIRED)

# Add sub directories
add_subdirectory(habit_msgs)
add_subdirectory(hvac_msgs)
add_subdirectory(seat_msgs)
add_subdirectory(window_msgs)
add_subdirectory(cabin_msgs)
add_subdirectory(cloud_msgs)
add_subdirectory(drive_msgs)
add_subdirectory(std_msgs)


add_subdirectory(battery_msgs)
