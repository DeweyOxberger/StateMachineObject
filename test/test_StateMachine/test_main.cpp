#include <unity.h>
#include <Arduino.h>
#include "StateMachine.h"

// Basic tests to make sure I didn't botch anything

// simple state machine that allows monitoring progress via some public data
class Machine1 : public StateMachine
{
public:
  Machine1();

  uint8_t data = 0;

  void useChild(StateMachine * child);

protected:

  void initialState(void);
  void state1(void);
};

Machine1::Machine1() : StateMachine(ACTION(initialState))  // StateMachine(ACTION(initialState))
{
  data = 1;
}

void Machine1::initialState(void)
{
  data = 2;
  setState(ACTION(state1));
}

void Machine1::state1(void)
{
  data = 3;
  done();
}

void Machine1::useChild(StateMachine * child)
{
  setChild(child);
}

// **** tests

Machine1 testObj;
Machine1 secondObj;

void setUp()
{

}

void tearDown()
{
  testObj.reset();
  secondObj.reset();
}

void test_first_test(void)  // getting the test framework up - just a sanity check
{
  int a = 2;
  TEST_ASSERT_EQUAL(2, a);
}

void test_create(void)
{
  TEST_ASSERT_EQUAL(1, testObj.data);
}

void test_initialState(void)
{
  testObj.update();
  TEST_ASSERT_EQUAL(2, testObj.data);
  TEST_ASSERT_EQUAL(false, testObj.isDone());
}

void test_next_state(void)
{
  testObj.update();
  testObj.update();
  TEST_ASSERT_EQUAL(3, testObj.data);
  TEST_ASSERT_EQUAL(true, testObj.isDone());
}

void test_child(void)
{
  testObj.update();
  testObj.useChild(&secondObj);
  testObj.update();
  TEST_ASSERT_EQUAL(2, testObj.data);
  TEST_ASSERT_EQUAL(2, secondObj.data);

  testObj.update();
  TEST_ASSERT_EQUAL(2, testObj.data);
  TEST_ASSERT_EQUAL(3, secondObj.data);
  TEST_ASSERT_EQUAL(true, secondObj.isDone());

  testObj.update();
  TEST_ASSERT_EQUAL(3, testObj.data);
  TEST_ASSERT_EQUAL(true, testObj.isDone());
  TEST_ASSERT_EQUAL(3, secondObj.data);
  TEST_ASSERT_EQUAL(true, secondObj.isDone());
}

void test_reset(void)
{
  testObj.update();  // 2
  testObj.update();  // 3
  testObj.reset();
  testObj.update();  // 2
  TEST_ASSERT_EQUAL(2, testObj.data);
}

//  ***** Arduino

void setup()
{
  Serial.begin(115200);
  while (!Serial)
  {}
  UNITY_BEGIN(); // IMPORTANT LINE!
  RUN_TEST(test_first_test);
  RUN_TEST(test_create);
  RUN_TEST(test_initialState);
  RUN_TEST(test_next_state);
  RUN_TEST(test_child);
  UNITY_END();
}

void loop()
{
  delay(10);
}

// Verbose Test will let the tests do:
// cout << "Hello" << endl;
// cout << "Hello\n";
// fprintf(stdout, "Hello");
// putchar('a');
// UnityPrint("Hello");
// UnityPrintLen("Print this, but not this", 10);
// UNITY_OUTPUT_CHAR('a');

// Project
//  Lib
//    Subdir
//      a.h
//      a.cpp