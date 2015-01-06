
#include "rpc_flow_control.h"
#include <string>

#include "base/barrier_closure.h"
#include "rpc_state.h"
#include "rpc_action.h"
#include "rpc_context.h"
#include "thirdparty/glog/logging.h"

void RpcActionRunner::RunAction(
    RpcContext* context, RpcAction* action, Closure* done) {
  context_ = context;
  action_ = action;
  CHECK(action_ != NULL);
  done_ = done;

  Closure* call_done = ::NewCallback(
      this, &RpcActionRunner::HandleActionDone);
  int ret = action->CallService(context, call_done);
  if (ret != kActionSucceed) {
    delete call_done;
    if (done != NULL) {
      done->Run();
    }
    delete this;
  }
}

void RpcActionRunner::HandleActionDone() {
  action_->ProcessResponse(context_);
  if (done_ != NULL) {
    done_->Run();
  }
  delete this;
}

void RpcStateRunner::RunState(
    RpcContext* context, RpcState* state,
    std::string* next_state_name, Closure* done) {
  context_ = context;
  state_ = state;
  CHECK(state_ != NULL);
  next_state_name_ = next_state_name;
  done_ = done;

  state->MakeUpActions(context, &state_actions_);
  Closure* state_done = ::NewCallback(
      this, &RpcStateRunner::HandleStateDone);
  Closure* barrier_done = new BarrierClosure(
      state_actions_.size() + 1, state_done);
  for (uint32_t i = 0; i < state_actions_.size(); ++i) {
    RpcActionRunner* action_runner = RpcActionRunner::Create();
    action_runner->RunAction(context, state_actions_[i].get(), barrier_done);
  }
  barrier_done->Run();
}

void RpcStateRunner::HandleStateDone() {
  std::string state_name = state_->Finish(context_, state_actions_);
  if (next_state_name_ != NULL) {
    *next_state_name_ = state_name;
  }
  if (done_ != NULL) {
    done_->Run();
  }
  delete this;
}

void RpcFlowControl::Run(RpcContext* context,
                         bool own_context,
                         Closure* done) {
  context_ = context;
  CHECK_NOTNULL(context_);
  own_context_ = own_context;
  done_ = done;
  start_state_ = context_->GetStartState();
  end_state_ = kRpcStateEnd;
  RunState(start_state_);;
}


void RpcFlowControl::Run(RpcContext* context,
                         bool own_context,
                         const std::string& start_state,
                         const std::string& end_state,
                         Closure* done) {
  context_ = context;
  CHECK_NOTNULL(context_);
  own_context_ = own_context;
  done_ = done;
  start_state_ = start_state;
  end_state_ = end_state;
  RunState(start_state_);
}

void RpcFlowControl::RunState(const std::string& state_name) {
  current_state_name_ = state_name;
  current_state_.reset(context_->CreateState(state_name));
  CHECK_NOTNULL(current_state_.get());
  VLOG(50) << "enter state:" << state_name;

  RpcStateRunner* state_runner = RpcStateRunner::Create();
  Closure* done = ::NewCallback(this, &RpcFlowControl::CurrentStateFinish);
  state_runner->RunState(context_, current_state_.get(),
                         &next_state_name_, done);
}

void RpcFlowControl::CurrentStateFinish() {
  VLOG(50) << "current state:" << current_state_name_ << " Finish,"
           << "next state:" << next_state_name_;
  if (current_state_name_ == end_state_ ||
      next_state_name_ == kRpcStateEnd) {
    VLOG(50) << "flow control finish.";
    if (done_ != NULL) {
      done_->Run();
    }
    if (own_context_) {
      delete context_;
      context_ = NULL;
    }
    delete this;
    return;
  }
  RunState(next_state_name_);
}

