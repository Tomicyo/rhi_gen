cmake_minimum_required(VERSION 3.0)
project(Sapphieiros)

if(POLICY CMP0054)
  cmake_policy(SET CMP0054 OLD)
endif()

list(APPEND CMAKE_MODULE_PATH "${Sapphieiros_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${Sapphieiros_SOURCE_DIR}/build/")

find_package(DX12)
find_package(VulkanSDK)
find_package(PythonInterp)
#find_package(Swig)
find_package(V8)

include_directories(
#${DX12_INCLUDE_DIR}
${V8_INCLUDE_DIR}
${VULKANSDK_INCLUDE_DIR}
${Sapphieiros_SOURCE_DIR}/src
${gtest_SOURCE_DIR}/include
${CMAKE_CURRENT_BINARY_DIR})

execute_process(COMMAND ${PYTHON_EXECUTABLE} ${Sapphieiros_SOURCE_DIR}/run.py
-o ${CMAKE_CURRENT_BINARY_DIR}/vulkan_impl.h ${Sapphieiros_SOURCE_DIR}/vulkan.yml
)

execute_process(COMMAND ${PYTHON_EXECUTABLE} ${Sapphieiros_SOURCE_DIR}/run.py
-o ${CMAKE_CURRENT_BINARY_DIR}/direct3d12_impl.h ${Sapphieiros_SOURCE_DIR}/direct3d12.yml
)
#execute_process(COMMAND ${SWIG_EXECUTABLE} -javascript -v8 -I${CMAKE_CURRENT_BINARY_DIR} -o ${CMAKE_CURRENT_BINARY_DIR}/v8_wrapper.cpp ${Sapphieiros_SOURCE_DIR}/sappheiros.i)

add_subdirectory(thirdparty/googletest)
add_subdirectory(src)
add_subdirectory(gtest)
add_subdirectory(data/test)

add_library(sapphire_vulkan SHARED ${CMAKE_CURRENT_BINARY_DIR}/vulkan_impl.cpp)
target_include_directories(sapphire_vulkan PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(sapphire_vulkan ${VULKAN_LIB})
set_target_properties(sapphire_vulkan PROPERTIES FOLDER "impl")

add_library(sapphire_direct3d12 SHARED ${CMAKE_CURRENT_BINARY_DIR}/direct3d12_impl.cpp)
target_include_directories(sapphire_direct3d12 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(sapphire_direct3d12 ${DX12_LIBRARIES})
set_target_properties(sapphire_direct3d12 PROPERTIES FOLDER "impl")

# add_executable(gtest_vulkan 
# gtest/test.cpp 
# gtest/implement_vk.h 
# gtest/implement_vk.cpp 
# gtest/implement_js_binding.h
# gtest/implement_js_binding.cpp
# #${CMAKE_CURRENT_BINARY_DIR}/v8_wrapper.cpp
# )
# target_include_directories(gtest_vulkan PRIVATE 
# ${CMAKE_CURRENT_BINARY_DIR} 
# ${V8_INCLUDE_DIR}
# app_js
# gtest 
# )
# target_compile_definitions(gtest_vulkan PRIVATE -DENABLE_V8=1)
# target_link_libraries(gtest_vulkan ${VULKAN_LIB} gtest ${V8_LIBRARIES} app_js)