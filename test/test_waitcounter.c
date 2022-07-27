#include "ecore/waitcounter.h"
#include "unity.h"

#ifdef USE_CEEDLING
void __attribute__((weak)) setUp() {}
void __attribute__((weak)) tearDown() {}
#endif

void test_TestWaitCounter_EmptyTake_() {
  WaitCounter WaitCounter;
  WaitCounter_init(&WaitCounter, WaitCounter_Binary);
  TEST_ASSERT_FALSE(WaitCounter_take(&WaitCounter, 0));
}

void test_TestWaitCounter_Counter_() {
  WaitCounter WaitCounter;
  static const int numberOfGiven = 10;
  WaitCounter_init(&WaitCounter, numberOfGiven);

  for (int i = 0; i < numberOfGiven; i++)
    WaitCounter_give(&WaitCounter);
  for (int i = 0; i < numberOfGiven; i++)
    TEST_ASSERT_TRUE(WaitCounter_take(&WaitCounter, 0));
}

void test_TestWaitCounter_Delay_() {
  WaitCounter WaitCounter;
  WaitCounter_init(&WaitCounter, WaitCounter_Binary);
  const clock_t startClock = clock();
  WaitCounter_take(&WaitCounter, 50);
  const clock_t endClock = clock();
  TEST_ASSERT(endClock > startClock);
}

#ifndef USE_CEEDLING
#include "unity_fixture.h"

TEST_GROUP(TestWaitCounter);

TEST_SETUP(TestWaitCounter) {}

TEST_TEAR_DOWN(TestWaitCounter) {}

TEST_GROUP_RUNNER(TestWaitCounter) {
  RUN_TEST_CASE(TestWaitCounter, EmptyTake);
  RUN_TEST_CASE(TestWaitCounter, Counter);
  RUN_TEST_CASE(TestWaitCounter, Delay);
}

TEST(TestWaitCounter, EmptyTake) { test_TestWaitCounter_EmptyTake_(); }

TEST(TestWaitCounter, Counter) { test_TestWaitCounter_Counter_(); }

TEST(TestWaitCounter, Delay) { test_TestWaitCounter_Delay_(); }

#endif