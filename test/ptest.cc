/**
 * @file test.cc
 * @author robin
 * @brief 
 *  这个是用来测试threadpool的类
 * @version 0.1
 * @date 2023-01-04
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <iostream>
#include "../thread-pool.h"

#include "../utils/ostreamlock.h"
#include "../utils/thread-utils.h"
#include "../utils/random-generator.h"

using namespace std;



int kNumMistion = 100;

struct RANDOM_GEN
{
  mutex rglock;
  RandomGenerator gen;
};


void basicTest()
{
  //构造线程池

  ThreadPool threadpool(100);
  RANDOM_GEN *rangen = new RANDOM_GEN();
  //派发任务
  for(int i = 0;i < kNumMistion;++i)
  {
    threadpool.schedule([i,rangen]{
      cout << oslock << "任务" << i << "开始执行" << endl << osunlock;
      rangen->rglock.lock();
      // sleep_for(rangen->gen.getNextInt(5,10) * 100);
      sleep_for(100);
      rangen->rglock.unlock();
      cout << oslock << "任务" << i << "结束" << endl << osunlock;
    });
  }

  threadpool.wait();
}

//验证多次等待是否可行
void mediumTest()
{
  //构造线程池
  
  ThreadPool threadpool(10);
  RANDOM_GEN *rangen = new RANDOM_GEN();
  //派发任务
  for(int i = 0;i < kNumMistion/2;++i)
  {
    threadpool.schedule([i,rangen]{
      cout << oslock << "任务" << i << "开始执行" << endl << osunlock;
      rangen->rglock.lock();
      // sleep_for(rangen->gen.getNextInt(5,10) * 100);
      sleep_for(100);
      rangen->rglock.unlock();
      cout << oslock << "任务" << i << "结束" << endl << osunlock;
    });
  }

  threadpool.wait();

  for(int i = kNumMistion/2;i < kNumMistion;++i)
  {
    threadpool.schedule([i,rangen]{
      cout << oslock << "任务" << i << "开始执行" << endl << osunlock;
      rangen->rglock.lock();
      // sleep_for(rangen->gen.getNextInt(5,10) * 100);
      sleep_for(100);
      rangen->rglock.unlock();
      cout << oslock << "任务" << i << "结束" << endl << osunlock;
    });
  }

  threadpool.wait();


}




int main()
{
  

  basicTest();
  mediumTest();
  return 0;
}