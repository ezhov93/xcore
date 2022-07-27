add_library(unity STATIC IMPORTED)
set_target_properties(unity PROPERTIES
  IMPORTED_LOCATION
  "C:/usr/lib/libunity.a"
  INTERFACE_INCLUDE_DIRECTORIES
  "C:/usr/include/unity/"
)

add_executable(testrun
  testrunner.c
  test_waitcounter.c
  test_circularbuffer.c
  test_softtimer.c
)

target_compile_features(testrun PRIVATE cxx_std_17 c_std_11)

target_link_libraries(testrun PRIVATE ECore unity)

add_test(NAME TestWaitCounter COMMAND testrun)
add_test(NAME TestCcircularBuffer COMMAND testrun)
add_test(NAME TestSoftTimer COMMAND testrun)

# add_test(NAME testuarttest COMMAND testrun)