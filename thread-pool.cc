/**
 * File: thread-pool.cc
 * --------------------
 * Presents the implementation of the ThreadPool class.
 */

#include <iostream>
#include <unistd.h>
#include "thread-pool.h"
using namespace std;

ThreadPool::ThreadPool(size_t numThreads):
wts(numThreads),
worksinfo(numThreads)
{
//启动所有的工人
for(int i = 0;i < numThreads;++i)
 {
   thread([i,this]{
    //先将这个工人标记为可用
    worksinfo[i].ISULOCK.lock();
      worksinfo[i].isUseAble = true;
    worksinfo[i].ISULOCK.unlock();

    //空闲工人的加上这个工人
    works.FWLOCK.lock();
      works.freeWorks.push(i);
    works.FWLOCK.unlock();

    //通知等待空闲工人的线程有人了
    works.freeNum.signal();

    //死循环等待工人结束
    while(true)
    {
      //等待调度器通知自己是否可以开始工作
      worksinfo[i].haswork.wait();

      //取出任务开始工作
      auto &task = worksinfo[i].task;
      task();

      //结束后线标记自己可以重新工作了
      worksinfo[i].ISULOCK.lock();
        worksinfo[i].isUseAble = true;
      worksinfo[i].ISULOCK.unlock();

      //空闲工人的加上这个工人
      works.FWLOCK.lock();
        works.freeWorks.push(i);
      works.FWLOCK.unlock();

      //通知等待空闲工人的线程有人了
      works.freeNum.signal();
    }


   });
 }

//启动调度器
dt = thread([this]{
  while(true)
  {
    misinfo.MISLOCK.lock();
    if()
    misinfo.MISLOCK.unlock();
  }
});

}


void ThreadPool::schedule(const function<void(void)>& thunk) 
{

}

/**
 * @brief 
 * 等待之前派发的任务被所有的线程完成
 */
void ThreadPool::wait() 
{
  
}

/**
 * @brief Destroy the Thread Pool:: Thread Pool object
 * 通知所有的线程可以结束了，然后让那些线程结束
 */
ThreadPool::~ThreadPool() 
{

}
