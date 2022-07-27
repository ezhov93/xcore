file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${ECore_SOURCE_DIR}/include/ecore/*.h")

add_library(ECore
        circularbuffer.c
        WaitCounter.c
        softtimer.c
        crc32.c
        log.c
        ${HEADER_LIST}
)

target_include_directories(ECore PUBLIC ../include)

target_link_libraries(ECore)

target_compile_features(ECore PUBLIC cxx_std_17 c_std_11)

source_group(
        TREE "${PROJECT_SOURCE_DIR}/include"
        PREFIX "Header Files"
        FILES ${HEADER_LIST})
