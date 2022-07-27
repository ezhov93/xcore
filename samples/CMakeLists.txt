add_executable(samples app.c)
target_compile_features(samples PRIVATE cxx_std_17 cxx_std_11)
target_link_libraries(samples PRIVATE ECore)
