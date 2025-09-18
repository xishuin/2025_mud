cmake_minimum_required(VERSION 3.15)

# 明确设置C和CXX编译器
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

project(MUDGameWindowsConsole CXX)

# 启用详细输出
set(CMAKE_VERBOSE_MAKEFILE ON)

# 为MinGW添加特定配置
if(MINGW)
    add_definitions(-D__GNUC__)
    # 跳过json.hpp的编译器版本检查
    add_definitions(-DJSON_SKIP_UNSUPPORTED_COMPILER_CHECK)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 解决std::byte与Windows头文件中byte的命名冲突
if(MINGW)
    # 定义NOMINMAX以避免Windows头文件中的min/max宏
    add_definitions(-DNOMINMAX)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    # 添加C++17支持以确保std::byte可用
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()


if(MSVC)
    # 对于 MSVC 编译器
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>") 
    add_compile_options(
        /MT$<$<CONFIG:Debug>:d>  # 静态链接运行时库
        /EHsc                   # 启用 C++ 异常处理
    )
else()
    # 对于 GCC/MinGW/Clang 编译器
    set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a") # 优先查找静态库
endif()

# 设置源文件与头文件路径（可根据你项目结构调整）

#include_directories(log)
set(SOURCES
        map/Map.cpp
        draw/DrawBasic.cpp
        draw/MainShell.cpp
        static/story/StorySystem.cpp
        main.cpp
        Role/Person_Skill_Class/Person.cpp
        Role/Person_Skill_Class/Skill.cpp
        Role/new_bag/bag.cpp
        Role/new_bag/Inventory.cpp
        Role/new_bag/Item.cpp
        map/map_data/map.cpp
        Role/Person_Skill_Class/monster.cpp
        Fight/Fight.cpp
        draw/GameSystem.cpp
        Role/new_bag/Shop.cpp
        Role/new_bag/Chest.cpp
        Role/new_bag/Equipment.cpp
)
# 添加可执行文件
add_executable(mud_game ${SOURCES})

target_include_directories(mud_game
    PRIVATE
        ${PROJECT_SOURCE_DIR}/map/map_data
        ${PROJECT_SOURCE_DIR}/map
        ${PROJECT_SOURCE_DIR}/Role/new_bag
        ${PROJECT_SOURCE_DIR}/draw
        ${PROJECT_SOURCE_DIR}/static/story
        ${PROJECT_SOURCE_DIR}/lib
        ${PROJECT_SOURCE_DIR}/Role/Person_Skill_Class
)

# 链接 Windows API 库（主要是 kernel32/user32/gdi32 等）
# 通常不需要手动写，除非使用特殊 API
# 如果你用的都是 <windows.h> 中 console API，系统默认链接即可

# 如果你使用 Unicode，请定义 UNICODE 宏（建议）
#target_compile_definitions(mud_game PRIVATE UNICODE _UNICODE)

# 如果你用 MSVC，可以设置控制台编码
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()
