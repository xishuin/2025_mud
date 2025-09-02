//
// Created by Lenovo on 25-9-2.
//
#ifndef TASK_H
#define TASK_H
#include <iostream>
#include <functional>
#include <utility>

// 通用的任务包装器类
// Task.h
#pragma once
#include <functional>
#include <type_traits>
#include <utility>

class Task {
public:
    using executor_type = std::function<void()>;
private:
    executor_type executor_;
public:
    Task() = default;
    template <typename Callable,
              typename = std::enable_if_t<!std::is_same_v<std::decay_t<Callable>, Task>>>
    Task(Callable&& callable) : executor_(std::forward<Callable>(callable)) {}
    template <typename Func, typename... Args>
    Task(Func&& func, Args&&... args) {
        executor_ = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
    }

    // 5. 移动构造/赋值
    Task(Task&&) noexcept = default;
    Task& operator=(Task&&) noexcept = default;

    // 6. 拷贝构造/赋值
    Task(const Task&) = default;
    Task& operator=(const Task&) = default;

    // 7. 调用操作符：执行任务
    void operator()() const {
        if (executor_) {
            executor_();
        }
    }

    // 8. 检查任务是否为空
    explicit operator bool() const noexcept {
        return static_cast<bool>(executor_);
    }

    // 9. 重置任务
    void reset() {
        executor_ = nullptr;
    }

    // 10. 交换
    void swap(Task& other) noexcept {
        executor_.swap(other.executor_);
    }
};

class Condition {
public:
    using predicate_type = std::function<bool()>;
private:
    predicate_type predicate_;
public:
    Condition() : predicate_([]() { return true; }) {}
    template <typename Callable,
              typename = std::enable_if_t<!std::is_same_v<std::decay_t<Callable>, Condition>>>
    explicit Condition(Callable&& callable) : predicate_(std::forward<Callable>(callable)) {}

    template <typename Func, typename... Args>
    explicit Condition(Func&& func, Args&&... args) {
        predicate_ = [func = std::forward<Func>(func), args...]() mutable -> bool {
            return func(args...);
        };
    }
    // 执行判断
    bool operator()() const {
        return predicate_();
    }
    // 显式转换为 bool，表示是否有效
    explicit operator bool() const noexcept {
        return static_cast<bool>(predicate_);
    }
    // 重置为默认（总是 true）
    void reset() {
        predicate_ = []() { return true; };
    }
    // 交换
    void swap(Condition& other) noexcept {
        predicate_.swap(other.predicate_);
    }
};

inline Condition operator&&(const Condition& a, const Condition& b) {
    return Condition([a, b]() -> bool {
        return a() && b();
    });
}

inline Condition operator||(const Condition& a, const Condition& b) {
    return Condition([a, b]() -> bool {
        return a() || b();
    });
}

inline Condition operator!(const Condition& a) {
    return Condition([a]() -> bool {
        return !a();
    });
}
#endif
