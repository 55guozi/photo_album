#include "threadpool.h"

ThreadPool::ThreadPool(int numThreads):stop(false)
{
    for(int i=0; i<numThreads; i++){
        threads.emplace_back([this](){
            while(true){
                std::unique_lock<std::mutex> lock(mtx);
                cv.wait(lock, [this](){return !tasks.empty() || stop;});
                if(stop && tasks.empty())return;

                auto task = std::move(tasks.front());
                tasks.pop();
                lock.unlock();
                task();
            }
        });
    }
}

ThreadPool::~ThreadPool(){
    {
        std::unique_lock<std::mutex> lock(mtx);
        stop = true;
    }
    cv.notify_all();
    for(auto& t : threads){
        t.join();
    }
}
