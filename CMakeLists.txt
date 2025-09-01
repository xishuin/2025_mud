cmake_minimum_required(VERSION 3.15)
project(MUDGameWindowsConsole)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置源文件与头文件路径（可根据你项目结构调整）
include_directories(Person_Skill_Class)
include_directories(bag)
include_directories(log)
file(GLOB_RECURSE SOURCES
        Map.cpp
        DrawBasic.cpp
        main.cpp
)
# 添加可执行文件
add_executable(mud_game ${SOURCES})



# 链接 Windows API 库（主要是 kernel32/user32/gdi32 等）
# 通常不需要手动写，除非使用特殊 API
# 如果你用的都是 <windows.h> 中 console API，系统默认链接即可



# 如果你使用 Unicode，请定义 UNICODE 宏（建议）
#target_compile_definitions(mud_game PRIVATE UNICODE _UNICODE)

# 如果你用 MSVC，可以设置控制台编码
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()
