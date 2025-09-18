#pragma once

// 这个头文件用于解决Windows头文件中byte与C++17 std::byte的命名冲突

#ifdef _WIN32
    // 保存原始的byte定义（如果存在）
    #ifdef byte
        #define _ORIGINAL_BYTE_DEFINED
        #undef byte
    #endif

    // 定义一个宏来安全地使用std::byte
    #include <cstddef>
    #define STD_BYTE std::byte

    // 恢复原始的byte定义
    #ifdef _ORIGINAL_BYTE_DEFINED
        #define byte unsigned char
        #undef _ORIGINAL_BYTE_DEFINED
    #endif
#else
    // 在非Windows系统上，直接使用std::byte
    #include <cstddef>
    #define STD_BYTE std::byte
#endif

// 使用示例:
// #include <windows.h>
// #include "byte_fix.hpp"
//
// // 在这里可以使用STD_BYTE代替std::byte
// STD_BYTE b = static_cast<STD_BYTE>(0x42);