#include "/usr/local/bundle/gems/ceedling-0.31.1/vendor/unity/src/unity.h"
#include "include/ecore/circularbuffer.h"




void __attribute__((weak)) setUp() {}

void __attribute__((weak)) tearDown() {}









static CircularBuffer circularBuffer_;

static int data_[7];



void test_TestCircularBuffer_Init_() {

  CircularBuffer_init(&circularBuffer_, data_, sizeof(data_), sizeof(data_[0]));



  UnityAssertEqualNumber((UNITY_INT)((circularBuffer_.data)), (UNITY_INT)(((void *)data_)), (

 ((void*)0)

 ), (UNITY_UINT)(17), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((UNITY_INT)((circularBuffer_.tail)), (UNITY_INT)((circularBuffer_.head)), (

 ((void*)0)

 ), (UNITY_UINT)(18), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((UNITY_INT)((circularBuffer_.elementSize)), (UNITY_INT)((sizeof(int))), (

 ((void*)0)

 ), (UNITY_UINT)(19), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((UNITY_INT)((circularBuffer_.max)), (UNITY_INT)((7)), (

 ((void*)0)

 ), (UNITY_UINT)(20), UNITY_DISPLAY_STYLE_INT);

  do {if (!(circularBuffer_.full)) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(21)));}} while(0);

}



void test_TestCircularBuffer_Push_() {

  for (size_t i = 0; i < 7; i++)

    do {if ((CircularBuffer_push(&circularBuffer_, &i))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(26)));}} while(0);

  UnityAssertEqualNumber((UNITY_INT)((CircularBuffer_size(&circularBuffer_))), (UNITY_INT)((7)), (

 ((void*)0)

 ), (UNITY_UINT)(27), UNITY_DISPLAY_STYLE_INT);

}



void test_TestCircularBuffer_PushOutOfBoundary_() {

  const int data = 0;

  do {if (!(CircularBuffer_push(&circularBuffer_, &data))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(32)));}} while(0);

  UnityAssertEqualNumber((UNITY_INT)((CircularBuffer_size(&circularBuffer_))), (UNITY_INT)((7)), (

 ((void*)0)

 ), (UNITY_UINT)(33), UNITY_DISPLAY_STYLE_INT);

}



void test_TestCircularBuffer_Pop_() {

  int data = 0;



  for (size_t i = 0; i < 7; i++) {

    do {if ((CircularBuffer_pop(&circularBuffer_, &data))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(40)));}} while(0);

    UnityAssertEqualNumber((UNITY_INT)((data)), (UNITY_INT)((i)), (

   ((void*)0)

   ), (UNITY_UINT)(41), UNITY_DISPLAY_STYLE_INT);

  }

  UnityAssertEqualNumber((UNITY_INT)((CircularBuffer_size(&circularBuffer_))), (UNITY_INT)((0)), (

 ((void*)0)

 ), (UNITY_UINT)(43), UNITY_DISPLAY_STYLE_INT);

  do {if ((CircularBuffer_isEmpty(&circularBuffer_))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(44)));}} while(0);

}



void test_TestCircularBuffer_PopOutOfBoundary_() {

  int data = 0;

  do {if (!(CircularBuffer_pop(&circularBuffer_, &data))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(49)));}} while(0);

  UnityAssertEqualNumber((UNITY_INT)((CircularBuffer_size(&circularBuffer_))), (UNITY_INT)((0)), (

 ((void*)0)

 ), (UNITY_UINT)(50), UNITY_DISPLAY_STYLE_INT);

}



void test_TestCircularBuffer_Peek_() {

  int data[7] = {0};



  for (int i = 0; i < 7; i++) {

    data[i] = 10 + i;

    do {if ((CircularBuffer_push(&circularBuffer_, &data[i]))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(58)));}} while(0);

  }



  int peek;

  for (int i = 0; i < 7; i++) {

    do {if ((CircularBuffer_peek(&circularBuffer_, &peek))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(63)));}} while(0);

    UnityAssertEqualNumber((UNITY_INT)((peek)), (UNITY_INT)((data[i])), (

   ((void*)0)

   ), (UNITY_UINT)(64), UNITY_DISPLAY_STYLE_INT);

    int tmp;

    do {if ((CircularBuffer_pop(&circularBuffer_, &tmp))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(66)));}} while(0);

  }



  do {if (!(CircularBuffer_peek(&circularBuffer_, &peek))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(69)));}} while(0);

}



void test_TestCircularBuffer_Size_() {

  UnityAssertEqualNumber((UNITY_INT)((CircularBuffer_size(&circularBuffer_))), (UNITY_INT)((0)), (

 ((void*)0)

 ), (UNITY_UINT)(73), UNITY_DISPLAY_STYLE_INT);

}



void test_TestCircularBuffer_Space_() {

  int data = 0;



  UnityAssertEqualNumber((UNITY_INT)((CircularBuffer_space(&circularBuffer_))), (UNITY_INT)((7)), (

 ((void*)0)

 ), (UNITY_UINT)(79), UNITY_DISPLAY_STYLE_INT);

  for (int i = 0; i < 7; i++) {

    UnityAssertEqualNumber((UNITY_INT)((CircularBuffer_size(&circularBuffer_))), (UNITY_INT)((i)), (

   ((void*)0)

   ), (UNITY_UINT)(81), UNITY_DISPLAY_STYLE_INT);

    do {if ((CircularBuffer_push(&circularBuffer_, &data))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(82)));}} while(0);

  }

  UnityAssertEqualNumber((UNITY_INT)((CircularBuffer_space(&circularBuffer_))), (UNITY_INT)((0)), (

 ((void*)0)

 ), (UNITY_UINT)(84), UNITY_DISPLAY_STYLE_INT);

}



void test_TestCircularBuffer_Clear_() {

  int data = 10;

  for (size_t i = 0; i < 7; i++)

    UnityAssertEqualNumber((UNITY_INT)((CircularBuffer_push(&circularBuffer_, &i))), (UNITY_INT)((0)), (

   ((void*)0)

   ), (UNITY_UINT)(90), UNITY_DISPLAY_STYLE_INT);

  CircularBuffer_clear(&circularBuffer_);

  UnityAssertEqualNumber((UNITY_INT)((CircularBuffer_size(&circularBuffer_))), (UNITY_INT)((0)), (

 ((void*)0)

 ), (UNITY_UINT)(92), UNITY_DISPLAY_STYLE_INT);

  do {if ((CircularBuffer_push(&circularBuffer_, &data))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(93)));}} while(0);

  UnityAssertEqualNumber((UNITY_INT)((CircularBuffer_size(&circularBuffer_))), (UNITY_INT)((1)), (

 ((void*)0)

 ), (UNITY_UINT)(94), UNITY_DISPLAY_STYLE_INT);

}

void test_TestCircularBuffer_Empty_() {

  int data = 10;

  CircularBuffer_clear(&circularBuffer_);

  do {if ((CircularBuffer_isEmpty(&circularBuffer_))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(99)));}} while(0);

  do {if ((CircularBuffer_push(&circularBuffer_, &data))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(100)));}} while(0);

  do {if (!(CircularBuffer_isEmpty(&circularBuffer_))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(101)));}} while(0);

  do {if ((CircularBuffer_pop(&circularBuffer_, &data))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(102)));}} while(0);

  do {if ((CircularBuffer_isEmpty(&circularBuffer_))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(103)));}} while(0);

}
