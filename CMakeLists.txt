cmake_minimum_required(VERSION 3.0)
project(Sapphieiros)

list(APPEND CMAKE_MODULE_PATH "${Sapphieiros_SOURCE_DIR}/cmake")

find_package(DX12)
find_package(VulkanSDK)
find_package(PythonInterp 3.5 REQUIRED)
set(USE_FOLDERS ON)

include_directories(
${DX12_INCLUDE_DIR}
${VULKANSDK_INCLUDE_DIR}
)

execute_process(COMMAND ${PYTHON_EXECUTABLE} run.py)

add_library(sapphire_vulkan SHARED ${CMAKE_CURRENT_BINARY_DIR}/vulkan_impl.cpp)
target_include_directories(sapphire_vulkan PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(sapphire_vulkan ${VULKAN_LIB})
set_target_properties(sapphire_vulkan PROPERTIES FOLDER "impl")

add_library(sapphire_direct3d12 SHARED ${CMAKE_CURRENT_BINARY_DIR}/direct3d12_impl.cpp)
target_include_directories(sapphire_direct3d12 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(sapphire_direct3d12 ${DX12_LIBRARIES})
set_target_properties(sapphire_direct3d12 PROPERTIES FOLDER "impl")

add_subdirectory(thirdparty/googletest)

add_executable(gtest_vulkan gtest/test.cpp gtest/implement.cpp)
target_include_directories(gtest_vulkan PRIVATE gtest)
target_link_libraries(gtest_vulkan ${VULKAN_LIB} gtest)