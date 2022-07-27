#include "ecore/circularbuffer.h"
#include "unity.h"

#ifdef USE_CEEDLING
void __attribute__((weak)) setUp() {}
void __attribute__((weak)) tearDown() {}
#endif

#define BUFFER_SIZE 7

static CircularBuffer circularBuffer_;
static int data_[BUFFER_SIZE];

void test_TestCircularBuffer_Init_() {
  CircularBuffer_init(&circularBuffer_, data_, sizeof(data_), sizeof(data_[0]));

  TEST_ASSERT_EQUAL(circularBuffer_.data, (void *)data_);
  TEST_ASSERT_EQUAL(circularBuffer_.tail, circularBuffer_.head);
  TEST_ASSERT_EQUAL(circularBuffer_.elementSize, sizeof(int));
  TEST_ASSERT_EQUAL(circularBuffer_.max, BUFFER_SIZE);
  TEST_ASSERT_FALSE(circularBuffer_.full);
}

void test_TestCircularBuffer_Push_() {
  for (size_t i = 0; i < BUFFER_SIZE; i++)
    TEST_ASSERT_TRUE(CircularBuffer_push(&circularBuffer_, &i));
  TEST_ASSERT_EQUAL(CircularBuffer_size(&circularBuffer_), BUFFER_SIZE);
}

void test_TestCircularBuffer_PushOutOfBoundary_() {
  const int data = 0;
  TEST_ASSERT_FALSE(CircularBuffer_push(&circularBuffer_, &data));
  TEST_ASSERT_EQUAL(CircularBuffer_size(&circularBuffer_), BUFFER_SIZE);
}

void test_TestCircularBuffer_Pop_() {
  int data = 0;

  for (size_t i = 0; i < BUFFER_SIZE; i++) {
    TEST_ASSERT_TRUE(CircularBuffer_pop(&circularBuffer_, &data));
    TEST_ASSERT_EQUAL(data, i);
  }
  TEST_ASSERT_EQUAL(CircularBuffer_size(&circularBuffer_), 0);
  TEST_ASSERT_TRUE(CircularBuffer_isEmpty(&circularBuffer_));
}

void test_TestCircularBuffer_PopOutOfBoundary_() {
  int data = 0;
  TEST_ASSERT_FALSE(CircularBuffer_pop(&circularBuffer_, &data));
  TEST_ASSERT_EQUAL(CircularBuffer_size(&circularBuffer_), 0);
}

void test_TestCircularBuffer_Peek_() {
  int data[BUFFER_SIZE] = {0};

  for (int i = 0; i < BUFFER_SIZE; i++) {
    data[i] = 10 + i;
    TEST_ASSERT_TRUE(CircularBuffer_push(&circularBuffer_, &data[i]));
  }

  int peek;
  for (int i = 0; i < BUFFER_SIZE; i++) {
    TEST_ASSERT_TRUE(CircularBuffer_peek(&circularBuffer_, &peek));
    TEST_ASSERT_EQUAL(peek, data[i]);
    int tmp;
    TEST_ASSERT_TRUE(CircularBuffer_pop(&circularBuffer_, &tmp));
  }

  TEST_ASSERT_FALSE(CircularBuffer_peek(&circularBuffer_, &peek));
}

void test_TestCircularBuffer_Size_() {
  TEST_ASSERT_EQUAL(CircularBuffer_size(&circularBuffer_), 0);
}

void test_TestCircularBuffer_Space_() {
  int data = 0;

  TEST_ASSERT_EQUAL(CircularBuffer_space(&circularBuffer_), BUFFER_SIZE);
  for (int i = 0; i < BUFFER_SIZE; i++) {
    TEST_ASSERT_EQUAL(CircularBuffer_size(&circularBuffer_), i);
    TEST_ASSERT_TRUE(CircularBuffer_push(&circularBuffer_, &data));
  }
  TEST_ASSERT_EQUAL(CircularBuffer_space(&circularBuffer_), 0);
}

void test_TestCircularBuffer_Clear_() {
  int data = 10;
  for (size_t i = 0; i < BUFFER_SIZE; i++)
    TEST_ASSERT_EQUAL(CircularBuffer_push(&circularBuffer_, &i), 0);
  CircularBuffer_clear(&circularBuffer_);
  TEST_ASSERT_EQUAL(CircularBuffer_size(&circularBuffer_), 0);
  TEST_ASSERT_TRUE(CircularBuffer_push(&circularBuffer_, &data));
  TEST_ASSERT_EQUAL(CircularBuffer_size(&circularBuffer_), 1);
}
void test_TestCircularBuffer_Empty_() {
  int data = 10;
  CircularBuffer_clear(&circularBuffer_);
  TEST_ASSERT_TRUE(CircularBuffer_isEmpty(&circularBuffer_));
  TEST_ASSERT_TRUE(CircularBuffer_push(&circularBuffer_, &data));
  TEST_ASSERT_FALSE(CircularBuffer_isEmpty(&circularBuffer_));
  TEST_ASSERT_TRUE(CircularBuffer_pop(&circularBuffer_, &data));
  TEST_ASSERT_TRUE(CircularBuffer_isEmpty(&circularBuffer_));
}

#ifndef USE_CEEDLING
#include "unity_fixture.h"

TEST_GROUP(TestCircularBuffer);

TEST_SETUP(TestCircularBuffer) {}

TEST_TEAR_DOWN(TestCircularBuffer) {}

TEST_GROUP_RUNNER(TestCircularBuffer) {
  RUN_TEST_CASE(TestCircularBuffer, Init);
  RUN_TEST_CASE(TestCircularBuffer, Push);
  RUN_TEST_CASE(TestCircularBuffer, PushOutOfBoundary);
  RUN_TEST_CASE(TestCircularBuffer, Pop);
  RUN_TEST_CASE(TestCircularBuffer, PopOutOfBoundary);
  RUN_TEST_CASE(TestCircularBuffer, Peek);
  RUN_TEST_CASE(TestCircularBuffer, Size);
  RUN_TEST_CASE(TestCircularBuffer, Space);
  RUN_TEST_CASE(TestCircularBuffer, Clear);
  RUN_TEST_CASE(TestCircularBuffer, Empty);
}

TEST(TestCircularBuffer, Init) { test_TestCircularBuffer_Init_(); }

TEST(TestCircularBuffer, Push) { test_TestCircularBuffer_Push_(); }

TEST(TestCircularBuffer, PushOutOfBoundary) {
  test_TestCircularBuffer_PushOutOfBoundary_();
}

TEST(TestCircularBuffer, Pop) { test_TestCircularBuffer_Pop_(); }

TEST(TestCircularBuffer, PopOutOfBoundary) {
  test_TestCircularBuffer_PopOutOfBoundary_();
}

TEST(TestCircularBuffer, Peek) { test_TestCircularBuffer_Peek_(); }

TEST(TestCircularBuffer, Size) { test_TestCircularBuffer_Size_(); }

TEST(TestCircularBuffer, Space) { test_TestCircularBuffer_Space_(); }

TEST(TestCircularBuffer, Clear) { test_TestCircularBuffer_Clear_(); }

TEST(TestCircularBuffer, Empty) { test_TestCircularBuffer_Empty_(); }

#endif
