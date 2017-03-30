#include "implement/implement_vk.h"
#include "js_implement/implement_js_binding.h"
#include "gtest/gtest.h"

int myArgc;
char ** myArgv;

TEST(JS_Binding, V8)
{
  ASSERT_EQ(v8_js_binding_test(myArgc, myArgv), 0);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  myArgc = argc;
  myArgv = argv;
  return RUN_ALL_TESTS();
}