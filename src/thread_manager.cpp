#include "thread_manager.hpp"
#include <vector>
#include <mutex>
#include <chrono>
#include <algorithm>

ThreadManager* ThreadManager::thread_manager_ptr = nullptr;
std::mutex q_mutex;
std::mutex thread_counter_mutex;
ThreadManager::ThreadManager(){
      if(!thread_manager_ptr){
            thread_manager_ptr = this;
            sys_threads = std::thread::hardware_concurrency(); 
            working_threads = sys_threads;
      }
      else
            delete this;
}
ThreadManager::ThreadManager(unsigned short thread_count){
      if(!thread_manager_ptr){
            thread_manager_ptr = this;
            sys_threads = thread_count; 
            working_threads = sys_threads;
      }
      else
            delete this;
}
void ThreadManager::function_caller()
{

    while (!end_of_thread)
    {
        std::shared_ptr<IFunct> task;
        while (1)
        {
            q_mutex.lock();
            if (!functionQ.empty() || end_of_thread)
                break;
            q_mutex.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        if (end_of_thread)
        {
            q_mutex.unlock();
            break;
        }

        task = functionQ.front();
        functionQ.front().reset();
        functionQ.pop();
        q_mutex.unlock();

        if (task)
            task->callFunction();
        if (task)
            task->onTaskEnd();
        task.reset();
    }
    thread_counter_mutex.lock();
    working_threads--;
    thread_counter_mutex.unlock();
}
void ThreadManager::add_function(std::shared_ptr<IFunct>funct){
      q_mutex.lock();
     
      functionQ.push (funct);
      q_mutex.unlock();
}
void ThreadManager::start_work(){
      if(!threads.empty()) return;
      
      for(int i = 0; i < sys_threads; i++){
            threads.push_back(std::thread(&ThreadManager::function_caller, ThreadManager::thread_manager_ptr));
            threads[i].detach();
      }
      
      
}
ThreadManager::~ThreadManager(){
      if(thread_manager_ptr == this){
            end_of_thread = true;
            while(working_threads){
                  
            }
            thread_manager_ptr = nullptr;
      }     
}
bool ThreadManager::has_work() const{
      return !functionQ.empty();
}