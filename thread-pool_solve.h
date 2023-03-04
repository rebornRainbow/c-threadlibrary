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

  struct workerTask{//这是每个工人得到自己工作的一个工具，表示线程的位置
    semaphore hasTask;
    std::mutex task_m;
    std::function<void(void)> task;//这里我打算用来装一个re
  };

  std::vector<workerTask> worker_task;

  struct Worker_and_schedule
  {
    semaphore freeworkers;//空闲的工人
    std::mutex add_task;//添加任务的锁
    std::queue<std::function<void(void)> >worktasks;//任务队列
    std::mutex add_workers_m;//添加工人的锁
    std::queue<int> workers;//空闲的工人队列
  } wo_a_sh;
               // 表示可用工人的数量
  std::mutex tasks_m;
  std::vector<std::function<void(void)> >tasks;
  semaphore addtask;

  std::mutex tasks_i_m;
  size_t tasks_i;
  size_t numThreads;

  //用来判断所有工人是否结束
  std::mutex finish_tasks_m;
  size_t finish_tasks = 0;
  std::condition_variable_any finish_tasks_s;
  struct ISEND{
    std::mutex end_m;
    bool end = false;
  } terminate;






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
