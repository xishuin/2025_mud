#pragma once

// 包含byte修复头文件
#include "byte_fix.hpp"

// 包含json.hpp头文件
#include "Person_Skill_Class/json.hpp"

// 确保使用C++17标准
#if __cplusplus < 201703L
    #error "json_wrapper.hpp requires C++17 or later"
#endif