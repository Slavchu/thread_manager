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
void ThreadManager::function_caller(){
      IFunct* task;

      while(!end_of_thread){
            
            while(1){
                  q_mutex.lock();
                  if(!functionQ.empty() && end_of_thread)
                  break;
                  q_mutex.unlock();
                  std::this_thread::sleep_for(std::chrono::milliseconds(1));       
            }
            if(end_of_thread){
                  q_mutex.unlock();
                  break;
                  
            }
            
            task = functionQ.front();
            functionQ.pop(); 
            q_mutex.unlock();
            
          

            task->callFunction();
            task->onTaskEnd();
            delete task;
      }
      thread_counter_mutex.lock();
      working_threads--;
      thread_counter_mutex.unlock();
}
void ThreadManager::add_function(IFunct *funct){
      q_mutex.lock();
      if(funct == nullptr) throw -1;
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