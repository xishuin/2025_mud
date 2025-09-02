//
// Created by Lenovo on 25-9-2.
//
#include <iostream>
#include <functional>
#include <utility>

// 通用的任务包装器类
class Task {
public:
    Task()=delete;
    template<typename Func, typename... Args>
    Task(Func&& func, Args&&... args) {
        task_ = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
    }
    // 执行任务
    void operator()() const {
        task_();
    }
private:
    std::function<void()> task_;
};
