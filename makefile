SEMAPHORE = utils/semaphore.cc
OSLOCK = utils/ostreamlock.cc
THUTIL = utils/thread-utils.cc
RANGEN = utils/random-generator.cc
THREADP = thread-pool.cc

GCC = g++ -Wall -std=c++11 -pthread 

TEST = test/ptest
TPTEST = test/tptest
TPCTEST = test/tpcustomtest

$(TEST):$(TEST).cc $(SEMAPHORE) $(OSLOCK) $(THUTIL) $(THREADP) $(RANGEN)
	$(GCC) $^ -o $@  

$(TPTEST):$(TPTEST).cc $(SEMAPHORE) $(OSLOCK) $(THUTIL) $(THREADP) $(RANGEN)
	$(GCC) $^ -o $@  

$(TPCTEST):$(TPCTEST).cc $(SEMAPHORE) $(OSLOCK) $(THUTIL) $(THREADP) $(RANGEN)
	$(GCC) $^ -o $@  


.PHONY:clean

clean:
	rm $(TEST) $(TPTEST) $(TPCTEST)