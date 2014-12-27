#include "barrier_closure.h"
#include <vector>
#include "callback.h"
#include "thirdparty/gtest/gtest.h"

static void DoInc(int *i) {
  ++(*i);
}

TEST(BarrierClosureTest, SimpleTest) {
  int i = 0;
  Closure* done = NewCallback(&DoInc, &i);
  Closure* barrier_done = new BarrierClosure(2, done);
  barrier_done->Run();
  EXPECT_EQ(0, i);
  barrier_done->Run();
  EXPECT_EQ(1, i);
}

TEST(BarrierClosureTest, MultipleThreadTest) {
  int i = 0;
  int kThreadNum = 1000;
  std::vector<shared_ptr<Thread> > threads;
  threads.resize(kThreadNum);
  Closure* done = NewCallback(&DoInc, &i);
  Closure* barrier_done = new BarrierClosure(kThreadNum + 1, done);
  for (int k = 0; k < kThreadNum; ++k) {
    threads[k].reset(new Thread(barrier_done));
    threads[k]->Start();
  }
  for (int k = 0; k < kThreadNum; ++k) {
    threads[k]->Join();
  }
  EXPECT_EQ(0, i);
  barrier_done->Run();
  EXPECT_EQ(1, i);
}
