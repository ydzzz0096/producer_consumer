// main.cpp -- 修正后的版本
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex> // 1. 包含 mutex 头文件
#include "threadsafe_queue.h"

using namespace std::chrono_literals;

std::mutex cout_mutex; // 2. 定义一个全局的打印锁

int main() 
{
    ThreadSafeQueue<int> q;

    // producer thread (这部分代码不用变)
    std::thread producer([&q]() {
        for (int i = 1; i <= 100; ++i) {
            q.push(i);
            std::this_thread::sleep_for(5ms);
        }
        std::this_thread::sleep_for(200ms);
        q.stop();
    });

    // 多个消费者
    int consumer_count = 3;
    std::vector<std::thread> consumers;
    for (int id = 0; id < consumer_count; ++id) {
        consumers.emplace_back([id, &q]() {
            while (true) {
                auto opt = q.pop();
                if (!opt.has_value()) break;
                int v = opt.value();
                std::this_thread::sleep_for(20ms);
                
                // 3. 在打印前后加锁
                {
                    std::lock_guard<std::mutex> lock(cout_mutex);
                    std::cout << "consumer " << id << " processed: " << v << std::endl;
                }
            }
            // 3. 在打印前后加锁
            {
                std::lock_guard<std::mutex> lock(cout_mutex);
                std::cout << "consumer " << id << " exiting" << std::endl;
            }
        });
    }

    producer.join();
    for (auto &t : consumers) t.join();

    std::cout << "all done" << std::endl;
    return 0;
}