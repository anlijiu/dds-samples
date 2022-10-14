include(CMakeFindDependencyMacro)

# 如果想要获取Config阶段的变量，可以使用这个
# set(my-config-var )

# 如果你的项目需要依赖其他的库，可以使用下面语句，用法与find_package相同
# find_dependency(MYDEP REQUIRED)

# Any extra setup

# Add the targets file
include("${CMAKE_CURRENT_LIST_DIR}/DataTypesBatteryTargets.cmake")
