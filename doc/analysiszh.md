## Milestone 1: Implementing the ThreadPool class
如何实现这个线程池？那么，您的 ThreadPool 构造函数——至少在最初——应该执行以下操作：

- 您可以使用以下方式启动单个分派线程（假设 dt 是私有 thread 数据成员）：
```c++
dt = thread([this]() {
  dispatcher();
});
```
- 您可以使用以下方式启动特定数量的工作线程（假设 wts 是私有 vector<thread> 数据成员）：

```c++
for (size_t workerID = 0; workerID < numThreads; workerID++) 
{
  wts[workerID] = thread([this](size_t workerID) 
  {
    worker(workerID);
  }, workerID);
}
```

实现调度应将提供的函数指针（表示为 function<void(void)> ，这是 C++ 中调用无参数函数的方式）附加到此类函数的队列末尾。每次安排函数时，应通知分派线程。一旦通知了分派程序，就应立即返回调度，以便可以安排更多函数。


题外话：不带任何参数的函数称为 thunks。 function<void(void)> 类型是比 void (*)() 更通用的类型，可以分配给任何可调用的函数（函数指针或匿名函数），而无需任何参数。

私有分派程序方法的实现应该几乎无限地循环，在每次迭代中阻塞，直到它确认未完成函数队列非空。然后，它应该等待工作线程可用，选择它，将其标记为不可用，出队最近调度的函数，将该函数的副本放在选定工作线程（仅那个工作线程）可以找到的地方，然后向工作线程发出信号以执行它。

私有工作线程方法的实现也应该重复循环，在每次迭代中阻塞，直到分派线程向其发出信号以执行分配的函数（如上所述）。一旦收到信号，工作线程就应该调用函数，等待其执行，然后将自己标记为可用，以便分派程序可以再次（再次，再次）发现并选择它。

wait 的实现应阻塞，直到所有先前安排但尚未执行的函数都已执行完毕。ThreadPool 析构函数应等待所有安排的函数完成执行，以某种方式通知分派线程和工作线程退出（并等待它们退出），然后释放所有 ThreadPool 资源。

我们的 ThreadPool 实现不应该遗留任何内存。我们将使用 valgrind 分析您的 ThreadPool，以确保不会泄漏内存。