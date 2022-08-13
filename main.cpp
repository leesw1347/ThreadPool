//
// Created by lsw94 on 22-08-13.
//


#include "pool/ThreadPool.h"

int main(int argc, char *argv[]) {
    ThreadPool::ThreadPool threadPool(3);
    std::cout << &threadPool << std::endl;

    for (int i = 0; i < 10; i++) {
        threadPool.EnqueueJob([i]() {
            work(i % 3, i);
        });
    }
    std::cout << "ThreadPool 메인 프로그램 종료" << std::endl;
}