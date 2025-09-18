#pragma once

#ifdef USE_STD_BYTE
    // 保存原始的byte定义
    #ifdef byte
        #define _ORIGINAL_BYTE_DEFINED
        #undef byte
    #endif

    // 定义byte为std::byte
    #include <cstddef>
    #define byte std::byte

    // 提供一个宏来恢复原始的byte定义
    // 提供一个宏来恢复原始的byte定义
    #define RESTORE_ORIGINAL_BYTE() \
        do { \
            #undef byte \
            #ifdef _ORIGINAL_BYTE_DEFINED \
                #define byte unsigned char \
                #undef _ORIGINAL_BYTE_DEFINED \
            #endif \
        } while(0)
#else
    // 如果不使用std::byte，则不做任何处理
    #define RESTORE_ORIGINAL_BYTE
#endif

// 使用示例:
// #include <windows.h>
// #include "byte_compat.h"
//
// // 在这里可以使用std::byte
// byte b = static_cast<byte>(0x42);
//
// // 如果需要使用Windows API中的byte，可以恢复原始定义
// RESTORE_ORIGINAL_BYTE
// byte windows_byte = 0x42;