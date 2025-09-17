#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include "threadsafe_queue.h"

using namespace std::chrono_literals;

int main() 

{
    ThreadSafeQueue<int> q;

    // producer thread
    std::thread producer([&q]() {
        for (int i = 1; i <= 100; ++i) {
            q.push(i);
            // 模拟生产间隔
            std::this_thread::sleep_for(5ms);
        }
        // 等待一会儿再停止，让消费者处理完
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
                if (!opt.has_value()) break; // stopped 且空
                int v = opt.value();
                // 模拟处理耗时
                std::this_thread::sleep_for(20ms);
                std::cout << "consumer " << id << " processed: " << v << std::endl;
            }
            std::cout << "consumer " << id << " exiting\n";
        });
    }

    producer.join();
    for (auto &t : consumers) t.join();

    std::cout << "all done\n";
    return 0;
}
