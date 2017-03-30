set(TARGET implement)
add_library(${TARGET} SHARED implement_vk.h implement_vk.cpp)
target_compile_definitions(${TARGET} PRIVATE -DBUILD_LIB=1)
target_link_libraries(${TARGET} ${VULKAN_LIB})