
#include <vector>
#include "rpc_flow_control.h"
#include "rpc_action.h"
#include "rpc_state.h"
#include "rpc_context.h"
#include "rpc/rpc_test_helper.h"
#include "thirdparty/gtest/gtest.h"
#include "thirdparty/glog/logging.h"

using namespace gdt::rpc::test;  // NOLINT

class TestContext : public RpcContext {
 public:
  void Init(google::protobuf::RpcController* controller,
            const IncRequest *request,
            IncResponse *response,
            google::protobuf::Closure *done) {
    controller_ = controller;
    request_ = request;
    response_ = response;
    done_ = done;
    if (request->count() == 1) {
      start_state_ = "DoubleState";
    } else if (request->count() == 2) {
      start_state_ = "TripleState";
    }
    for (uint32_t i = 0; i < 6; i++) {
      ss.push_back(1);
    }
  }
  std::string GetStartState() {
    return start_state_;
  }

 public:
  google::protobuf::RpcController* controller_;
  const IncRequest* request_;
  IncResponse* response_;
  google::protobuf::Closure* done_;
  string start_state_;
  vector<int> ss;
};

// 乘2 action
class DoubleAction : public RpcAction {
 public:
  void Init(int* input) {
    input_ = input;
  }

  virtual int CallService(RpcContext* context, Closure* done) {
    *input_ = (*input_) * 2;
    done->Run();
    return 0;
  }
  virtual void ProcessResponse(RpcContext* context) {
  }
 private:
  int* input_;
};
REGISTER_RPC_ACTION(DoubleAction);

// 乘3 action
class TripleAction : public RpcAction {
 public:
  void Init(int* input) {
    input_ = input;
  }
  virtual int CallService(RpcContext* context, Closure* done) {
    *input_ = (*input_) * 3;
    done->Run();
    return 0;
  }
  virtual void ProcessResponse(RpcContext* context) {
  }
 private:
  int* input_;
};
REGISTER_RPC_ACTION(TripleAction);

class DoubleState : public RpcState {
 public:
  void MakeUpActions(RpcContext* context,
                     std::vector<shared_ptr<RpcAction> >* actions) {
    TestContext* tcontext = dynamic_cast<TestContext*>(context);
    for (int i = 0; i < 6; i++) {
      if (i % 2 == 0) {
        shared_ptr<DoubleAction> action(new DoubleAction);
        action->Init(&tcontext->ss[i]);
        actions->push_back(action);
      }
    }
  }
  std::string Finish(RpcContext* context,
                     const std::vector<shared_ptr<RpcAction> >& actions) {
    TestContext* tcontext = dynamic_cast<TestContext*>(context);
    int sum = 0;
    for (uint32_t i = 0; i < 6; ++i) {
      sum += tcontext->ss[i];
    }
    tcontext->response_->set_result(sum);
    return "TripleState";
  }
};

REGISTER_RPC_STATE(DoubleState);

class TripleState : public RpcState {
 public:
  void MakeUpActions(RpcContext* context,
                     std::vector<shared_ptr<RpcAction> >* actions) {
    TestContext* tcontext = dynamic_cast<TestContext*>(context);
    for (int i = 0; i < 6; i++) {
      if (i % 2 == 1) {
        shared_ptr<TripleAction> action(new TripleAction);
        action->Init(&tcontext->ss[i]);
        actions->push_back(action);
      }
    }
  }
  std::string Finish(RpcContext* context,
                     const std::vector<shared_ptr<RpcAction> >& actions) {
    TestContext* tcontext = dynamic_cast<TestContext*>(context);
    int sum = 0;
    for (uint32_t i = 0; i < 6; ++i) {
      sum += tcontext->ss[i];
    }
    tcontext->response_->set_result(sum);
    tcontext->done_->Run();
    return kRpcStateEnd;
  }
};

REGISTER_RPC_STATE(TripleState);


// 服务端实现方式
class ServerTestService : public TestService {
 public:
  virtual void Inc(google::protobuf::RpcController *controller,
                   const IncRequest *request,
                   IncResponse *response,
                   google::protobuf::Closure *done) {
    TestContext* context = new TestContext();
    context->Init(controller, request, response, done);
    RpcFlowControl* flow_control = RpcFlowControl::Create();
    flow_control->Run(context);
  }
  virtual void Echo(google::protobuf::RpcController *controller,
                    const EchoRequest *request,
                    EchoResponse *response,
                    google::protobuf::Closure *done) {
  }
};

void SetFlag(bool* flag) {
  *flag = true;
}

TEST(RpcFlowControlTest, Test0) {
  scoped_ptr<ServerTestService> test_service;
  test_service.reset(new ServerTestService);
  IncRequest request;
  request.set_count(1);  // 从DoubleState开始
  request.set_step(1);
  IncResponse response;
  bool flag = false;
  Closure* done = NewCallback(&SetFlag, &flag);
  test_service->Inc(NULL, &request, &response, done);
  EXPECT_EQ(15, response.result());
  EXPECT_TRUE(flag);

  flag = false;
  request.set_count(2);  // 从TripleState开始
  IncResponse response2;
  Closure* done2 = NewCallback(&SetFlag, &flag);
  test_service->Inc(NULL, &request, &response2, done2);
  EXPECT_EQ(12, response2.result());
  EXPECT_TRUE(flag);
}

TEST(RpcFlowControlTest, RunSpecifiedState) {
  IncRequest request;
  request.set_count(1);
  request.set_step(1);
  IncResponse response;
  TestContext* context = new TestContext();
  context->Init(NULL, &request, &response, NULL);
  RpcFlowControl* flow_control = RpcFlowControl::Create();
  // 只跑DoubleState
  flow_control->Run(context, true, "DoubleState", "DoubleState");
  EXPECT_EQ(9, response.result());
}

TEST(RpcFlowControlTest, RpcStateRunnerTest) {
  IncRequest request;
  request.set_count(1);
  request.set_step(1);
  IncResponse response;
  TestContext context;
  context.Init(NULL, &request, &response, NULL);
  RpcStateRunner* state_runner = RpcStateRunner::Create();
  DoubleState double_state;
  std::string next_state_name;
  // 只跑DoubleState
  state_runner->RunState(&context, &double_state, &next_state_name, NULL);
  EXPECT_EQ(9, response.result());
  EXPECT_EQ("TripleState", next_state_name);
}

TEST(RpcFlowControlTest, RpcActionRunnerTest) {
  DoubleAction double_action;
  int result = 1;
  double_action.Init(&result);
  RpcActionRunner* action_runner = RpcActionRunner::Create();
  // 只跑DoubleAction
  action_runner->RunAction(NULL, &double_action, NULL);
  EXPECT_EQ(2, result);
}
