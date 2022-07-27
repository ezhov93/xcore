#include "/usr/local/bundle/gems/ceedling-0.31.1/vendor/unity/src/unity.h"
#include "include/ecore/softtimer.h"






void __attribute__((weak)) setUp() {}

void __attribute__((weak)) tearDown() {}





void test_TestSoftTimer_isReady_() {

  static const int delayMs = 500;

  SoftTimer timer;

  SoftTimer_init(&timer);

  SoftTimer_start(&timer, delayMs);

  const clock_t startClock = clock();

  static const clock_t tickDelay =

      (((double)delayMs + 1) * (

                               1000000L 

                                              / 1000));

  clock_t endClock = clock() + tickDelay;



  

 _Bool 

      error = 

              0

                   ;



  static const int numberOfTriggers = 3;



  int countOfTriggers = 0;



  for (int i = 0; i < numberOfTriggers; i++) {

    while (SoftTimer_isReady(&timer) == 

                                       0

                                            ) {

      if (clock() > endClock) {

        error = 

               1

                   ;

        break;

      }

    }

    if (error == 

                1

                    )

      break;

    endClock = clock() + tickDelay;

    countOfTriggers++;

  }



  do {if (!(error)) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(39)));}} while(0);

  do {if ((countOfTriggers == numberOfTriggers)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(40)));}} while(0);

}



void test_TestSoftTimer_singleShot_() {

}



void test_TestSoftTimer_callback_() {

}
