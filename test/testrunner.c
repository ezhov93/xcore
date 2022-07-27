#include <unity_fixture.h>

static void _runAllTests() {
  RUN_TEST_GROUP(TestWaitCounter);
  RUN_TEST_GROUP(TestCircularBuffer);
  RUN_TEST_GROUP(TestSoftTimer);
}

int main(int argc, const char *argv[]) {
  return UnityMain(argc, argv, _runAllTests);
}