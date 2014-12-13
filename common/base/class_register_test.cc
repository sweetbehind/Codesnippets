
#include "feeds/test/class_register.h"
#include "thirdparty/gtest/gtest.h"

class BaseClass {
 public:
  BaseClass() {}
  virtual ~BaseClass() {}
  virtual std::string call() = 0;
};

CLASS_REGISTER_DEFINE_REGISTRY(test_register, BaseClass);

#define REGISTER_CLASS(sub_class) \
  CLASS_REGISTER_OBJECT_CREATOR( \
      test_register, BaseClass, #sub_class, sub_class) \

#define CREATE_CLASS(sub_class_name) \
  CLASS_REGISTER_CREATE_OBJECT(test_register, sub_class_name)

class SubClass1 : public BaseClass {
 public:
  SubClass1() {}
  ~SubClass1() {}
  virtual std::string call() {
    return "SubClass1";
  }
};

REGISTER_CLASS(SubClass1);

class SubClass2 : public BaseClass {
 public:
  SubClass2() {}
  ~SubClass2() {}
  virtual std::string call() {
    return "SubClass2";
  }
};

REGISTER_CLASS(SubClass2);

// inherit from SubClass1
class SubClass3 : public SubClass1 {
 public:
  SubClass3() {}
  ~SubClass3() {}
  virtual std::string call() {
    return "SubClass3";
  }
};

REGISTER_CLASS(SubClass3);

TEST(ClassRegisterTest, Test0) {
  BaseClass* sub_class1 = CREATE_CLASS("SubClass1");
  BaseClass* sub_class2 = CREATE_CLASS("SubClass2");
  BaseClass* sub_class3 = CREATE_CLASS("SubClass3");
  EXPECT_EQ("SubClass1", sub_class1->call());
  EXPECT_EQ("SubClass2", sub_class2->call());
  EXPECT_EQ("SubClass3", sub_class3->call());
}
