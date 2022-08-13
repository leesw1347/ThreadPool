//
// Created by lsw94 on 22-08-13.
//


#include "pool/ThreadPool.h"

int main(int argc, char *argv[]) {
    ThreadPool::ThreadPool threadPool(3);
    std::cout << &threadPool << std::endl;

    // future<int>를 담는 객체를 futures vector에 담는다.
    std::vector<std::future<int>> futures;

    for (int i = 0; i < 10; i++) {
        futures.emplace_back(threadPool.EnqueueJob(work, i % 3 + 1, i));
    }
    std::cout << futures.size() << std::endl;

    /*for (int i = 0; i < 10; i++) {
        threadPool.EnqueueJob([i]() {
        });
    }*/
    std::cout << "ThreadPool 메인 프로그램 종료" << std::endl;
}