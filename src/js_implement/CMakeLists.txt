set(TARGET js_implement)
add_library(${TARGET} SHARED implement_js_binding.h implement_js_binding.cpp)
target_compile_definitions(${TARGET} PRIVATE -DBUILD_JS_IMPL_LIB=1)
target_include_directories(${TARGET} PRIVATE ${V8_INCLUDE_DIR})
target_link_libraries(${TARGET} ${V8_LIBRARIES} implement app js_app)