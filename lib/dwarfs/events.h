 
#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <iostream>
#include "alias.h"

namespace dw {

struct Call {
   dw::func<void(void*)> proc; // function to exec. void* - arguments
   dw::func<void(void*)> free_args; // pointer to tuple delete operator function. void* - arguments
   void* args = nullptr; // Function arguments

   Call(){};

   // move 
   Call(Call && other ) :
      proc(other.proc),
      free_args(other.free_args),
      args(other.args)
   {
      other.proc = nullptr; 
      other.free_args = nullptr; 
      other.args = nullptr; 
   }

   // copy move
   Call& operator=(Call&& other)
   {
      proc = other.proc;
      free_args = other.free_args;
      args = other.args;

      other.proc = nullptr; 
      other.free_args = nullptr; 
      other.args = nullptr; 
      return *this;
   }

   void exec()
   {
      proc(args); 
   };

   ~Call()
   {
      if(!args)
         return;
      
      if(!free_args){
          std::cout << "Don't know how to free args" << std::endl;
          std::exit(0);
      }

      free_args(args);
   }
};

template <typename T>
void deleter(void* d)
{
   delete (T*)d;
}
template <typename ...Args>
Call make_call(dw::func<void(void*)> const& p, Args const& ... args)
{
   Call b;
   b.args = new dw::tup<Args...>(args...);
   b.free_args = deleter<dw::tup<Args...>>;
   b.proc = p; // found from connection mapping

   return std::move(b);
};

using EVENT_ID = int;
using Queue = dw::arr<dw::func<void()>>;


struct EventListener
{
   std::atomic_bool stop = false;
   std::mutex mtx;
   std::condition_variable cond_v;
   dw::deq<Call> queue;

   ~EventListener()
   {
      std::cout << "Listener destroyed" << std::endl;
   }

   void eventLoop(){
      while(!stop){

      Call call;

      { // lock scope
         std::unique_lock<std::mutex> lck(mtx);
         cond_v.wait(lck);
        
         // after the wait, we own the lock.
         if(queue.empty())// spurious wakeup check
            continue;

         call = std::move(queue[0]); // todo make fifo
         queue.pop_front();
      }//lock destructor called

      call.exec();
      // lz::print(queue.size());
      }
      std::cout << "Listener stopped" << std::endl;
   }

   void pushCall(Call call)
   {
      std::unique_lock<std::mutex> lk(mtx);
      queue.emplace_back(std::move(call));
      cond_v.notify_one();
   }
};


struct Connection {
   EVENT_ID event_id;
   dw::func<void(void*)> exec_func;
   EventListener* listener;
};

extern dw::arr<Connection> connections;
extern std::atomic_int8_t event_Id_generator;


template <typename ...Args>
struct Signal {
   EVENT_ID event_id;

   Signal() : event_id(event_Id_generator){ event_Id_generator++; }

   auto emit(Args&... args)
   {
      auto a = (sizeof...(args));

      for(auto& c : connections){
         if(event_id == c.event_id)
            c.listener->pushCall(make_call(c.exec_func, args...));
      }
   }
};

template <typename T, typename MemProc, typename ... Ts>
auto connect(Signal<Ts...> const& signal, T* instance, MemProc mp, EventListener* listener)
{
   auto exec_func = [=](void* d){
      auto call = [&](Ts... args) {
         (instance->*(mp))(args...);
      };
      std::apply(call, (*(dw::tup<Ts...>*)d));
   };

   Connection conn;
   conn.event_id = signal.event_id;
   conn.exec_func = std::move(exec_func);
   conn.listener = listener;
   connections.push_back(conn);
}

// todo not tested!
template <typename T, typename Proc, typename ... Ts>
auto connect(Signal<Ts...> const& signal, Proc proc, EventListener* listener)
{
    auto exec_func = [=](void* d){
      auto call = [&](Ts... args) {
         proc(args...);
      };
      std::apply(call, (*(dw::tup<Ts...>*)d));
   };

   Connection conn;
   conn.event_id = signal.event_id;
   conn.exec_func = std::move(exec_func);
   conn.listener = listener;
   connections.push_back(conn);
}

}
