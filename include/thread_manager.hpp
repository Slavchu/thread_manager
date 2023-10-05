#pragma once
#include <thread>
#include <queue>
#include <functional>
#include <memory>
class IFunct{
    public:
    virtual void callFunction() = 0;
    virtual void onTaskEnd() = 0;    
};


class ThreadManager{

private:
    static ThreadManager * thread_manager_ptr;
    unsigned short sys_threads;
    std::queue<std::shared_ptr<IFunct>> functionQ;
    void function_caller();
    std::vector <std::thread> threads;
    int working_threads;
    bool end_of_thread = false;
    
public:
    void add_function(std::shared_ptr<IFunct> funct);
    ThreadManager();
    ThreadManager (unsigned short thread_count); 
    void start_work();
    bool has_work() const;
    ~ThreadManager();
};