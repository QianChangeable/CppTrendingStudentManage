cmake_minimum_required(VERSION 3.16)
project(CppTrendingStudentManage LANGUAGES CXX)

# 使用 C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 源文件
add_executable(StudentManage
    src/main.cpp
    src/Student.cpp
    include/Student.h
)

# 让编译器能找到 include 目录下的头文件
target_include_directories(StudentManage PRIVATE include)

target_compile_options(StudentManage PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/utf-8>
)

