
#ifndef RPC_STATE_H_
#define RPC_STATE_H_

#include <vector>
#include <string>
#include "base/class_register.h"

const char kRpcStateEnd[] = "rpc_state_end";

class RpcAction;
class RpcContext;

class RpcState {
 public:
  RpcState() { }
  virtual ~RpcState() { }
  // 返回需要调用的action列表
  virtual void MakeUpActions(
      RpcContext* context,
      std::vector<shared_ptr<RpcAction> >* actions) = 0;

  // 所有actions结束后的处理; 返回值是下个state的名字
  // state_string="rpc_state_end"是一个特殊的state，表示流程结束
  virtual std::string Finish(
      RpcContext* context,
      const std::vector<shared_ptr<RpcAction> >& actions) = 0;
};

CLASS_REGISTER_DEFINE_REGISTRY(rpc_state_register, RpcState);

#define REGISTER_RPC_STATE(state_name) \
  CLASS_REGISTER_OBJECT_CREATOR( \
    rpc_state_register, RpcState, #state_name, state_name) \

#define CREATE_RPC_STATE(state_name_as_string) \
  CLASS_REGISTER_CREATE_OBJECT(rpc_state_register, state_name_as_string)

#endif  // RPC_STATE_H_

