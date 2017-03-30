set(TARGET test_implement)
add_executable(${TARGET} test_implement.cpp)
target_link_libraries(${TARGET} app implement js_implement js_app gtest)

set(TARGET2 test_js_implement)
add_executable(${TARGET2} test_js_implement.cpp)
target_link_libraries(${TARGET2} app implement js_implement js_app gtest)