//
// Created by lsw94 on 22-08-13.
//

#ifndef THREADPOOL_THREADPOOL_H
#define THREADPOOL_THREADPOOL_H

#include <iostream>
#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <memory>
#include <typeinfo>
#include <future>

namespace ThreadPool {

    class ThreadPool {
    private:
        // 총 Worker 쓰레드의 개수
        size_t num_threads_;

        // Worker 쓰레드를 보관하는 벡터
        std::vector<std::thread> worker_threads_;

        // 할일들을 보관하는 job 큐.
        std::queue<std::function<void()>> jobs_;

        // 위의 job 큐를 위한 cv 와 m.
        std::condition_variable cv_job_q;
        std::mutex m_job_q;

        // 모든 쓰레드 종료
        bool stop_all;

        // Worker 쓰레드
        void WorkerThread();

    public:
        explicit ThreadPool(size_t num_threads);

        ThreadPool();

        virtual ~ThreadPool();

        /*job을 추가한다*/
//        void EnqueueJob(const std::function<void()> &job);

        /*job을 추가한다*/
        template<class F, class ... Args>
        std::future<typename std::result_of<F(Args ...)>::type> EnqueueJob(
                F f, Args... args
        );

    };

} // ThreadPool

int work(int t, int id);

#endif //THREADPOOL_THREADPOOL_H











