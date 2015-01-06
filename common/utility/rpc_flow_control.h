
// RpcFlowControl:跑整个rpc流程的各个state
// RpcActionRunner:只跑指定action
// RpcStateRunner:只跑指定state

#ifndef RPC_FLOW_CONTROL_H_
#define RPC_FLOW_CONTROL_H_

#include <vector>
#include <string>
#include "base/callback.h"
#include "base/shared_ptr.h"
#include "base/barrier_closure.h"
#include "rpc_action.h"
#include "rpc_state.h"
#include "thirdparty/glog/logging.h"

class RpcContext;

class RpcActionRunner {
 public:
  static RpcActionRunner* Create() {
    return new RpcActionRunner();
  }
  ~RpcActionRunner() {}
  void RunAction(RpcContext* context,
                 RpcAction* action,
                 Closure* done);

 private:
  RpcActionRunner() : context_(NULL), action_(NULL), done_(NULL) {}
  void HandleActionDone();

 private:
  RpcContext* context_;
  RpcAction* action_;
  Closure* done_;
};

class RpcStateRunner {
 public:
  static RpcStateRunner* Create() {
    return new RpcStateRunner;
  }
  ~RpcStateRunner() {}
  void RunState(RpcContext* context,
                RpcState* state,
                std::string* next_state_name,
                Closure* done);

 private:
  RpcStateRunner()
    : context_(NULL), state_(NULL),
      next_state_name_(NULL), done_(NULL) {}
  void HandleStateDone();

 private:
  RpcContext* context_;
  RpcState* state_;
  std::string* next_state_name_;
  Closure* done_;
  std::vector<shared_ptr<RpcAction> > state_actions_;
};

class RpcFlowControl {
 public:
  static RpcFlowControl* Create() {
    return new RpcFlowControl();
  }
  ~RpcFlowControl() {}

  // 调用Run(context) 可以跑context的整个请求过程,
  // 例子参考rpc_flow_control_test.cc
  // 一般own_context和done都使用默认值
  void Run(RpcContext* context,
           bool own_context = true,
           Closure* done = NULL);

  // 调用Run(context, start_state, end_state)
  // 只跑指定的state(包含end_state)，只是测试可能用到
  //
  void Run(RpcContext* context,
           bool own_context,
           const std::string& start_state,
           const std::string& end_state,
           Closure* done = NULL);

 private:
  RpcFlowControl()
    : context_(NULL),
      own_context_(true),
      done_(NULL) { }

  void RunState(const std::string& state_name);

  void CurrentStateFinish();

 private:
  RpcContext* context_;
  bool own_context_;
  Closure* done_;

  std::string start_state_;
  std::string end_state_;
  shared_ptr<RpcState> current_state_;
  std::string current_state_name_;
  std::string next_state_name_;
};

#endif  // RPC_FLOW_CONTROL_H_
