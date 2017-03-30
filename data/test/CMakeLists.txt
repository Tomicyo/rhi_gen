set(TEST_JS_SRCS sample.js triangle.js)
source_group(Scripts FILES ${TEST_JS_SRCS})
add_custom_target(test_script SOURCES ${TEST_JS_SRCS})