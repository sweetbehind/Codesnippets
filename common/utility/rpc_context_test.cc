
#include <vector>
#include "rpc_context.h"
#include "rpc_action.h"
#include "rpc_state.h"
#include "thirdparty/gtest/gtest.h"

class TestAction : public RpcAction {
 public:
  std::string test_name() {
    return "test_action";
  }

  virtual int CallService(RpcContext* context, Closure* done) {
    return kActionSucceed;
  }

  virtual void ProcessResponse(RpcContext* context) { }
};
REGISTER_RPC_ACTION(TestAction);

class TestState : public RpcState {
 public:
  std::string test_name() {
    return "test_state";
  }

  void MakeUpActions(RpcContext* context,
                     std::vector<shared_ptr<RpcAction> >* actions) { }

  std::string Finish(RpcContext* context,
                     const std::vector<shared_ptr<RpcAction> >& actions) {
    return "test";
  }
};
REGISTER_RPC_STATE(TestState);

class TestContext : public RpcContext {
 public:
  std::string GetStartState() {
    return "test";
  }
};

TEST(RpcContextTest, CreateActionTest) {
  TestContext test_context;
  RpcAction* action = test_context.CreateAction("TestAction");
  EXPECT_FALSE(NULL == action);
  TestAction* test_action = dynamic_cast<TestAction*>(action);
  EXPECT_EQ("test_action", test_action->test_name());
  delete action;
}

TEST(RpcContextTest, CreateStateTest) {
  TestContext test_context;
  RpcState* state = test_context.CreateState("TestState");
  EXPECT_FALSE(NULL == state);
  TestState* test_state = dynamic_cast<TestState*>(state);
  EXPECT_EQ("test_state", test_state->test_name());
  delete state;
}

