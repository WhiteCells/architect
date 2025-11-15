cmake_minimum_required(VERSION 3.10)

project(eventbus)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC
    src/model_a/model_a.cc
    src/model_b/model_b.cc
    src/thread_pool/thread_pool.cc
    src/main.cc
)

add_executable(${PROJECT_NAME} ${SRC})
