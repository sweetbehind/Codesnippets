
#include <string>
#include "base/clock.h"
#include "rpc_context.h"
#include "rpc_action.h"
#include "rpc_state.h"

RpcState* RpcContext::CreateState(const std::string& state_name) {
  RpcState* state = CREATE_RPC_STATE(state_name);
  CHECK_NOTNULL(state);
  return state;
}

RpcAction* RpcContext::CreateAction(const std::string& action_name) {
  RpcAction* action = CREATE_RPC_ACTION(action_name);
  CHECK_NOTNULL(action);
  return action;
}

RpcContext::RpcContext() {
  start_time_ = gdt::MonotonicClock::MilliSeconds();
}

int RpcContext::GetElapsedTime() const {
  return gdt::MonotonicClock::MilliSeconds() - start_time_;
}

