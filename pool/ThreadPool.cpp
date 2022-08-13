//
// Created by lsw94 on 22-08-13.
//

#include "ThreadPool.h"

namespace ThreadPool {
    ThreadPool::ThreadPool() {
        std::cout << "ThreadPool 디폴트 생성자 호출" << std::endl;
    }

    ThreadPool::ThreadPool(size_t num_threads)
            : num_threads_(num_threads), stop_all(false) {
        std::cout << "ThreadPool 클래스 생성자 호출 num_threads(" << num_threads_ << ")" << std::endl;
        worker_threads_.reserve(num_threads_);

        // num_threads만큼 Worker Thread에 워커함수를 추가한다
        for (size_t i = 0; i < num_threads_; i++) {
            // Worker Thread Vector에 lambda 함수를 추가한다
            worker_threads_.emplace_back(
                    [this]() {
                        this->WorkerThread();
                    }
            );
        }
    }

    ThreadPool::~ThreadPool() {
        std::cout << "ThreadPool 클래스 소멸자 호출" << std::endl;
        stop_all = true;
        cv_job_q.notify_all(); // 모든 쓰레드들을 실행 시켜서 종료 시킨다

        for (auto &k: worker_threads_) {
            if (k.joinable()) {
                k.join();
            }
        }
    }

    /**
     *
     * @param job: queue에 입력하기 위한 std::function 객체 void job() {} 형태로 함수를 넣는다

    void ThreadPool::EnqueueJob(const std::function<void()> &job) {
        if (stop_all) {
            throw std::runtime_error("ThreadPool 사용 중지됨");
        }
        {
            std::lock_guard<std::mutex> lockGuard(m_job_q);
            jobs_.push(std::move(job));
        }
        cv_job_q.notify_one(); // WorkerThread에서 잠자고 있는 쓰레드 중 하나를 깨워서 함수를 실행 시킨다
    }
      */

    template<class F, class... Args>
    std::future<typename std::result_of<F(Args...)>::type> ThreadPool::EnqueueJob(F f, Args... args) {
        if (stop_all) {
            throw std::runtime_error("ThreadPool 사용중지 됨");
        }

        using return_type = typename std::result_of<F(Args...)>::type;
        // return_type()은 함수 F(Args...) 의 리턴 값을 type을 결정한다;
        std::packaged_task<return_type()> job(std::bind(f, args...));
        std::future<return_type> job_result_future = job.get_future();
        {
            std::lock_guard<std::mutex> lockGuard(m_job_q);
            jobs_.push([&job]() {
                job();
            });
        }
        cv_job_q.notify_one(); // job 함수가 실행되고, jobs_ std::queue에 데이터가 들어가면
        // 데이터를 넣고, cv.wait에서 자고있는 쓰레드를 깨워서 WorkerThread 함수가 작동하도록 전달해준다.
        return job_result_future;
    }

    /**
     * job_queue에서 데이터를 끄집어 내서 함수를 실행 시킨다
     */
    void ThreadPool::WorkerThread() {
        while (true) {
            std::unique_lock<std::mutex> lock(m_job_q);
            cv_job_q.wait(lock, [this]() {
                return !this->jobs_.empty() || stop_all;
            });

            if (stop_all && this->jobs_.empty()) {
                return;
            }

            // 맨 앞의 job을 뺀다
            std::function<void()> job = std::move(jobs_.front());
            jobs_.pop();
            lock.unlock();

            // 해당 job을 수행한다
            job.operator()();
        }
    }

} // ThreadPool;
int work(int t, int id) {
    printf("%d start \n", id);
    std::this_thread::sleep_for(std::chrono::seconds(t));
    printf("%d end after %ds\n", id, t);
}