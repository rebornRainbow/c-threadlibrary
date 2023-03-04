/**
 * File: thread-pool.h
 * -------------------
 * This class defines the ThreadPool class, which accepts a collection
 * of thunks (which are zero-argument functions that don't return a value)
 * and schedules them in a FIFO manner to be executed by a constant number
 * of child threads that exist solely to invoke previously scheduled thunks.
 */

#ifndef _thread_pool_
#define _thread_pool_

#include <cstddef>     // for size_t
#include <functional>  // for the function template used in the schedule signature
#include <thread>      // for thread
#include <vector>      // for vector
#include <atomic>      // for atomic
#include <queue>       // for tasks
#include <mutex>       // 这是锁

#include "utils/semaphore.h"

class Debug
{

};

class ThreadPool {
 public:

/**
 * 构造一个线程池
 */
  ThreadPool(size_t numThreads);

/**
 * 一旦所有之前调度thunk已经被处理完以后
 * 调度提供的thunk(可以作为一个没有参数和返回值得函数调用)
 * 作为线程池的一个线程执行
 */
  void schedule(const std::function<void(void)>& thunk);

/**
 * 阻塞和等待直到所有之前调度的thunks完全执行
 */
  void wait();

/**
 * 等待所有之前调度的thunks执行，
 * 然后，适当地关闭ThreadPool和在其生命周期内使用的任何资源。
 */
  ~ThreadPool();
  
 private:
 
  std::thread dt;                // 派发线程处理
  std::vector<std::thread> wts;  // 工人线程处理器

//这里是工人相关的变量

  //这是单个工人的
  struct WORKINFO{
    //标记这个工人是否可用
    bool isUseAble = false;//是否可用
    mutex ISULOCK;//保护isUseAble的锁
    std::function<void(void)>& task;//标记自己的任务
    semaphore haswork;//工人和调度器之间通知自己是否有任务
  };

  vector<WORKINFO> worksinfo;//所有工人的信息

  //这是所有工人的
  struct ALLWORKINFO{
    mutex FWLOCK;//保护freeworks的锁
    queue<int> freeWorks;//标记空闲工人的标记
    semaphore freeNum;//标记空闲工人数量信号量
  }works;


//这里是和需要解决的任务有关的东西
struct MISINFO{
  mutex MISLOCK;//保护mission的锁
  queue<std::function<void(void)>& > missions;//管理需要解决的任务
  semaphore misNum;//需要解决的任务的数量
}misinfo;

//什么时候退出，没任务了，然后调度器





//这里是用来阻止复制的
/**
 * ThreadPools are the type of thing that shouldn't be cloneable, since it's
 * not clear what it means to clone a ThreadPool (should copies of all outstanding
 * functions to be executed be copied?).
 *
 * In order to prevent cloning, we remove the copy constructor and the
 * assignment operator.  By doing so, the compiler will ensure we never clone
 * a ThreadPool.
 */
  ThreadPool(const ThreadPool& original) = delete;
  ThreadPool& operator=(const ThreadPool& rhs) = delete;
};

#endif
