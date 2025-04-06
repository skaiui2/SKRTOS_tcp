cmake_minimum_required(VERSION 3.10)
project(NetStack)

set(CMAKE_C_STANDARD 99)

# 设置构建类型为 Debug
set(CMAKE_BUILD_TYPE Debug)


# 添加编译选项以包含调试符号
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")

# 添加头文件目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/app/include
    ${CMAKE_CURRENT_SOURCE_DIR}/core/include
    ${CMAKE_CURRENT_SOURCE_DIR}/net/include
    ${CMAKE_CURRENT_SOURCE_DIR}/port/include
    
)

# 使用 file(GLOB ...) 获取目录中的所有源文件
file(GLOB LWIP_APP_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/app/source/*.c")
file(GLOB LWIP_CORE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/core/source/*.c")
file(GLOB LWIP_NET_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/net/source/*.c")
file(GLOB LWIP_PORT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/port/source/*.c")

# 打印收集到的文件（用于调试）
message("LWIP APP Sources: ${LWIP_APP_SOURCES}")
message("LWIP Core Sources: ${LWIP_CORE_SOURCES}")
message("LWIP Net Sources: ${LWIP_NET_SOURCES}")
message("LWIP Port Sources: ${LWIP_PORT_SOURCES}")

# 定义目标可执行文件
add_executable(NetStack  
    testfile/test.c
    ${LWIP_APP_SOURCES}
    ${LWIP_CORE_SOURCES} 
    ${LWIP_NET_SOURCES} 
    ${LWIP_PORT_SOURCES}
    
)

# 链接线程库
target_link_libraries(NetStack pthread)

add_custom_target(cleanup
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/CMakeFiles
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/CMakeCache.txt
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/NetStack
    COMMENT "Cleaning up build files"
)
