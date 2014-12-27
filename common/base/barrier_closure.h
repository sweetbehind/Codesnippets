#ifndef BARRIER_CLOSURE_H_
#define BARRIER_CLOSURE_H_

#include "callback.h"
#include "thirdparty/perftools/atomicops.h"

// 继承Closure回调类，支持不带参数的成员函数Run()
// 用途：调用left_次BarrierClosure::Run后调用done_Run
class BarrierClosure : public Closure {
 public:
  BarrierClosure(int n, Closure* done)
    : left_(n), done_(done) { }

  ~BarrierClosure() { }

  virtual bool IsRepeatable() const {
    return false;
  }

  void Run() {
    // 调用left_次BarrierClosure::Run()后调用done_的Run()
    if (0 == base::subtle::Barrier_AtomicIncrement(&left_, -1)) {
      done_->Run();
      delete this;
    }
  }

 private:
  int left_;
  Closure* done_;
  DISALLOW_COPY_AND_ASSIGN(BarrierClosure);
};

#endif  // BARRIER_CLOSURE_H_
