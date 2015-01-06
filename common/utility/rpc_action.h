
#ifndef RPC_ACTION_H_
#define RPC_ACTION_H_

#include <string>
#include "base/class_register.h"
#include "base/callback.h"

class RpcContext;

enum ActionErrorCode {
  kActionFailed = -1,
  kActionSucceed = 0,
};

class RpcAction {
 public:
  RpcAction() { }
  virtual ~RpcAction() { }
  // 这里不用打包，只是直接调用下游rpc服务
  // 参数closure要传给rpc调用的closure，
  // 回调时会进入this::ProcessResponse。
  //
  // 若函数内部出错，没有调用下游rpc，则有两个选择：
  // 1.也执行done->Run(),return kActionSucceed:主动进入processResponse
  // 2.return kActionFailed:结束action，不会在进入processResponse，一般是这种情况
  //
  // CallService(RpcContext* context, Closure* done) {
  //   search_req = MakeUpRequest(context);
  //   if (search_req.valid()) {
  //    SearchService.Search(RpcController, search_req, &context->search_rsp, done)
  //    return kActionSucceed;
  //   } else {
  //    return kActionFailed; // 结束action
  //   }
  // }
  virtual int CallService(RpcContext* context, Closure* done) = 0;

  // 这里也不用对结果解包,只是拿到结果后的进一步处理逻辑
  // 处理完成后请调用done->Run()，例子：
  // ProcessResponse(RpcContext *context, Closure *done) {
  //  Log(INFO) << context->search_rsp.DebugString();
  //  done->Run();
  // }
  virtual void ProcessResponse(RpcContext* context) = 0;

  void set_action_name(const std::string& name) {
    action_name_ = name;
  }
  const std::string& action_name() {
    return action_name_;
  }

 protected:
  std::string action_name_;
};

CLASS_REGISTER_DEFINE_REGISTRY(rpc_action_register, RpcAction);

#define REGISTER_RPC_ACTION(action_name) \
  CLASS_REGISTER_OBJECT_CREATOR( \
    rpc_action_register, RpcAction, #action_name, action_name) \

#define CREATE_RPC_ACTION(action_name_as_string) \
  CLASS_REGISTER_CREATE_OBJECT(rpc_action_register, action_name_as_string)

#endif  // RPC_ACTION_H_

