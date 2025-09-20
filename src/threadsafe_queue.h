#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>

template <typename T>
class ThreadSafeQueue {
public:
    ThreadSafeQueue() = default;
    
    // push value into queue
    void push(T value) {
        {
            std::lock_guard<std::mutex> lk(m_);
            q_.push(std::move(value));
        }
        cv_.notify_one();
    }

    // pop blocks until item or stop called; returns std::nullopt if stopped and empty
    std::optional<T> pop() {
        std::unique_lock<std::mutex> lk(m_);
        cv_.wait(lk, [this]{ return !q_.empty() || stopped_; });

        // 关键修改：只要队列不为空，就优先处理数据
        if (!q_.empty()) {
            T v = std::move(q_.front());
            q_.pop();
            return v;
        }

        // 如果代码能执行到这里，说明队列一定是空的。
        // 而能从 wait 唤醒，只可能是因为 stopped_ == true。
        // 所以直接返回 nullopt 即可。
        return std::nullopt;
    }

    void stop() {
        {
            std::lock_guard<std::mutex> lk(m_);
            stopped_ = true;
        }
        cv_.notify_all();
    }

    size_t size() {
        std::lock_guard<std::mutex> lk(m_);
        return q_.size();
    }

private:
    std::queue<T> q_;
    std::mutex m_;
    std::condition_variable cv_;
    bool stopped_{false};
};