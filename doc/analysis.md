## Milestone 1: Implementing the ThreadPool class
How does one implement this thread pool thing? Well, your ThreadPool constructor—at least
initially—should do the following:
- launch a single dispatcher thread like this (assuming dt is a private thread data member)
```c++
dt = thread([this]() {
  dispatcher();
});
```
- launch a specific number of worker threads like this (assuming wts is a private vector<thread> data member):

```c++
for (size_t workerID = 0; workerID < numThreads; workerID++) 
{
  wts[workerID] = thread([this](size_t workerID) 
  {
    worker(workerID);
  }, workerID);
}
```

The implementation of schedule should append the provided function pointer (expressed as a function<void(void)> , which is the C++ way to classify a function that can be invoked with-out any arguments) to the end of a queue of such functions. Each time a function is scheduled, the dispatcher thread should be notified. Once the dispatcher has been notified, schedule should return right away so even more functions can be scheduled.


Aside: Functions that take no arguments at all are called thunks. The function<void(void)> type is a more general type than void (*)(), and can be assigned to anything invokable—a function pointer, or an anonymous function—that doesn’t require any arguments.

The implementation of the private dispatcher method should loop almost interminably, blocking within each iteration until it has confirmation the queue of outstanding functions is nonempty.It should then wait for a worker thread to become available, select it, mark it as unavailable, dequeue the least recently scheduled function, put a copy of that function in a place where the selected worker (and only that worker) can find it, and then signal the worker thread to execute it.

The implementation of the private worker method should also loop repeatedly, blocking within each iteration until the dispatcher thread signals it to execute an assigned function (as described above). Once signaled, the worker should go ahead and invoke the function, wait for it to execute,and then mark itself as available so that it can be discovered and selected again (and again, and again) by the dispatcher.

The implementation of wait should block until all previously-scheduled-but-yet-to-be-executed functions have been executed. The ThreadPool destructor should wait until all scheduled functions have executed to completion, somehow inform the dispatcher and worker threads to exit (and wait for them to exit), and then otherwise dispose of all ThreadPool resources.

our ThreadPool implementation shouldn’t orphan any memory whatsoever. We'll be analyzing your ThreadPool using valgrind to ensure no memory is leaked.
