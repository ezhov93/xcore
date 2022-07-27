#include "ecore/softtimer.h"
#include "unity.h"
#include <time.h>

#ifdef USE_CEEDLING
void __attribute__((weak)) setUp() {}
void __attribute__((weak)) tearDown() {}
#endif

void test_TestSoftTimer_isReady_() {
  static const int delayMs = 500;
  SoftTimer timer;
  SoftTimer_init(&timer);
  SoftTimer_start(&timer, delayMs);
  const clock_t startClock = clock();
  static const clock_t tickDelay =
      (((double)delayMs + 1) * (CLOCKS_PER_SEC / 1000));
  clock_t endClock = clock() + tickDelay;

  bool error = false;

  static const int numberOfTriggers = 3;

  int countOfTriggers = 0;

  for (int i = 0; i < numberOfTriggers; i++) {
    while (SoftTimer_isReady(&timer) == false) {
      if (clock() > endClock) {
        error = true;
        break;
      }
    }
    if (error == true)
      break;
    endClock = clock() + tickDelay;
    countOfTriggers++;
  }

  TEST_ASSERT_FALSE(error);
  TEST_ASSERT(countOfTriggers == numberOfTriggers);
}

void test_TestSoftTimer_singleShot_() {
  /* WaitCounter WaitCounter;
   WaitCounter_initialize(&WaitCounter);
   static const int numberOfGiven = 10;

   for (int i = 0; i < numberOfGiven; i++)
     WaitCounter_give(&WaitCounter);
   for (int i = 0; i < numberOfGiven; i++)
     TEST_ASSERT_TRUE(WaitCounter_take(&WaitCounter, 0));
     */
}

void test_TestSoftTimer_callback_() {
  /*
  WaitCounter WaitCounter;
  WaitCounter_initialize(&WaitCounter);
  const clock_t startClock = clock();
  WaitCounter_take(&WaitCounter, 50);
  const clock_t endClock = clock();
  TEST_ASSERT(endClock > startClock);
  */
}

#ifndef USE_CEEDLING
#include "unity_fixture.h"

TEST_GROUP(TestSoftTimer);

TEST_SETUP(TestSoftTimer) {}

TEST_TEAR_DOWN(TestSoftTimer) {}

TEST_GROUP_RUNNER(TestSoftTimer) {
  RUN_TEST_CASE(TestSoftTimer, isReady);
  RUN_TEST_CASE(TestSoftTimer, singleShot);
  RUN_TEST_CASE(TestSoftTimer, callback);
}

TEST(TestSoftTimer, isReady) { test_TestSoftTimer_isReady_(); }

TEST(TestSoftTimer, singleShot) { test_TestSoftTimer_singleShot_();
static const char *commandStr[] = {"p, clk, dac, pass"}; 

 }

TEST(TestSoftTimer, callback) { test_TestSoftTimer_callback_(); }

#endif