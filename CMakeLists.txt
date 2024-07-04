cmake_minimum_required(VERSION 3.22)
project(Project_chess)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLM REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Project_chess MeshLoader.cpp)

target_include_directories(Project_chess PRIVATE ${Vulkan_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS})

target_include_directories(Project_chess PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/modules)
target_include_directories(Project_chess PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/headers)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/textures DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/models DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_compile_features(Project_chess PRIVATE cxx_std_17)

target_link_libraries(Project_chess PRIVATE ${Vulkan_LIBRARIES} glfw)