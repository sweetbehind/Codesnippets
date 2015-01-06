
#ifndef RPC_CONTEXT_H_
#define RPC_CONTEXT_H_

#include <stdint.h>
#include <string>

class RpcAction;
class RpcState;

// 保留整个请求流转过程中需要的上下文数据结构
class RpcContext {
 public:
  RpcContext();
  // eplapsed time in ms
  int GetElapsedTime() const;
  virtual ~RpcContext() { }
  // 在请求初始化后，确定第一个要进入的state
  virtual std::string GetStartState() = 0;
  // 由rpc_flow_control调用，创建该请求的一个state实例
  virtual RpcState* CreateState(const std::string& state_name);
  // 各个state可以通过CreateAction来创建action，也可以各个state直接new action
  virtual RpcAction* CreateAction(const std::string& action_name);

 private:
  int64_t start_time_;
};

#endif  // RPC_CONTEXT_H_

