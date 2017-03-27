cmake_minimum_required(VERSION 3.0)
project(Sapphieiros)

if(POLICY CMP0054)
  cmake_policy(SET CMP0054 OLD)
endif()

list(APPEND CMAKE_MODULE_PATH "${Sapphieiros_SOURCE_DIR}/cmake")

find_package(DX12)
find_package(VulkanSDK)
find_package(PythonInterp 3.5 REQUIRED)
#find_package(Swig)
find_package(V8)
set(USE_FOLDERS ON)

include_directories(
#${DX12_INCLUDE_DIR}
${VULKANSDK_INCLUDE_DIR}
)

execute_process(COMMAND ${PYTHON_EXECUTABLE} run.py)
#execute_process(COMMAND ${SWIG_EXECUTABLE} -javascript -v8 -I${CMAKE_CURRENT_BINARY_DIR} -o ${CMAKE_CURRENT_BINARY_DIR}/v8_wrapper.cpp ${Sapphieiros_SOURCE_DIR}/sappheiros.i)

add_library(sapphire_vulkan SHARED ${CMAKE_CURRENT_BINARY_DIR}/vulkan_impl.cpp)
target_include_directories(sapphire_vulkan PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(sapphire_vulkan ${VULKAN_LIB})
set_target_properties(sapphire_vulkan PROPERTIES FOLDER "impl")

add_library(sapphire_direct3d12 SHARED ${CMAKE_CURRENT_BINARY_DIR}/direct3d12_impl.cpp)
target_include_directories(sapphire_direct3d12 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(sapphire_direct3d12 ${DX12_LIBRARIES})
set_target_properties(sapphire_direct3d12 PROPERTIES FOLDER "impl")

add_subdirectory(thirdparty/googletest)

add_executable(gtest_vulkan 
gtest/test.cpp 
gtest/implement_vk.h 
gtest/implement_vk.cpp 
gtest/implement_js_binding.h
gtest/implement_js_binding.cpp
#${CMAKE_CURRENT_BINARY_DIR}/v8_wrapper.cpp
)

target_include_directories(gtest_vulkan PRIVATE 
${CMAKE_CURRENT_BINARY_DIR} 
${V8_INCLUDE_DIR}
gtest 
)

target_link_libraries(gtest_vulkan ${VULKAN_LIB} gtest ${V8_LIBRARIES})