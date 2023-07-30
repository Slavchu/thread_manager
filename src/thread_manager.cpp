#include "thread_manager.hpp"
#include <vector>
#include <mutex>
#include <chrono>
#include <algorithm>
thread_manager* thread_manager::thread_manager_ptr = nullptr;
std::mutex q_mutex;
std::mutex thread_counter_mutex;
thread_manager::thread_manager(){
      if(!thread_manager_ptr){
            thread_manager_ptr = this;
            sys_threads = std::thread::hardware_concurrency(); 
            working_threads = sys_threads;
      }
      else
            delete this;
}
void thread_manager::function_caller(){
      while(!end_of_thread){
            q_mutex.lock();
            
            while(functionQ.empty() && !end_of_thread){
                  std::this_thread::sleep_for(std::chrono::milliseconds(1));       
            }
            if(end_of_thread){
                  q_mutex.unlock();
                  break;
                  
            }
            
            IFunct* task = functionQ.front();
            functionQ.pop(); 
            q_mutex.unlock();
            
          

            task->callFunction();
            task->onTaskEnd();
          
      }
      thread_counter_mutex.lock();
      working_threads--;
      thread_counter_mutex.unlock();
}
void thread_manager::add_function(IFunct *funct){
      q_mutex.lock();
      if(funct == nullptr) throw -1;
      functionQ.push (funct);
      q_mutex.unlock();
}
void thread_manager::start_work(){
      if(!threads.empty()) return;
      
      for(int i = 0; i < sys_threads; i++){
            threads.push_back(std::thread(&thread_manager::function_caller, thread_manager::thread_manager_ptr));
            threads[i].detach();
      }
      
      
}
thread_manager::~thread_manager(){
      if(thread_manager_ptr == this){
            end_of_thread = true;
            while(working_threads){
                  
            }
            thread_manager_ptr = nullptr;
      }     
}
bool thread_manager::has_work() const{
      return !functionQ.empty();
}