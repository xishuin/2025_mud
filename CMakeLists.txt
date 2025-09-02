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

# 设置源文件与头文件路径（可根据你项目结构调整）

#include_directories(log)
set(SOURCES
        Map.cpp
        DrawBasic.cpp
        StorySystem.cpp
        main.cpp
        Person_Skill_Class/Person.cpp
        Person_Skill_Class/Skill.cpp
        new_bag/bag.cpp
        new_bag/Inventory.cpp
        new_bag/Item.cpp
        map/map.cpp
)
# 添加可执行文件
add_executable(mud_game ${SOURCES})

target_include_directories(mud_game
    PRIVATE
        ${PROJECT_SOURCE_DIR}/map
        ${PROJECT_SOURCE_DIR}/new_bag
        ${PROJECT_SOURCE_DIR}/Person_Skill_Class
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
