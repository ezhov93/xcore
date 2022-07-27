#include "/usr/local/bundle/gems/ceedling-0.31.1/vendor/unity/src/unity.h"
#include "include/ecore/waitcounter.h"




void __attribute__((weak)) setUp() {}

void __attribute__((weak)) tearDown() {}





void test_TestWaitCounter_EmptyTake_() {

  WaitCounter WaitCounter;

  WaitCounter_init(&WaitCounter, WaitCounter_Binary);

  do {if (!(WaitCounter_take(&WaitCounter, 0))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(12)));}} while(0);

}



void test_TestWaitCounter_Counter_() {

  WaitCounter WaitCounter;

  static const int numberOfGiven = 10;

  WaitCounter_init(&WaitCounter, numberOfGiven);



  for (int i = 0; i < numberOfGiven; i++)

    WaitCounter_give(&WaitCounter);

  for (int i = 0; i < numberOfGiven; i++)

    do {if ((WaitCounter_take(&WaitCounter, 0))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(23)));}} while(0);

}



void test_TestWaitCounter_Delay_() {

  WaitCounter WaitCounter;

  WaitCounter_init(&WaitCounter, WaitCounter_Binary);

  const clock_t startClock = clock();

  WaitCounter_take(&WaitCounter, 50);

  const clock_t endClock = clock();

  do {if ((endClock > startClock)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(32)));}} while(0);

}
