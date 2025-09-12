#include <cassert>
#include <thread>
#include <vector>
#include "threadsafe_queue.h"
#include <iostream> // 新增

int main() {
    ThreadSafeQueue<int> q;
    // 测试 push/pop 单线程
    q.push(42);
    auto v = q.pop();
    assert(v.has_value() && v.value() == 42);

    // 测试多 producer/consumer 小场景
    ThreadSafeQueue<int> q2;
    std::thread p([&]{ for (int i=0; i<10; i++) q2.push(i); q2.stop(); });
    std::vector<int> got;
    std::thread c([&]{ while (true) { auto o=q2.pop(); if(!o) break; got.push_back(*o); }});
    p.join();
    c.join();
    assert(got.size() == 10);

    // 添加这一行
    std::cout << "所有测试成功通过！" << std::endl; 
    
    return 0;
}