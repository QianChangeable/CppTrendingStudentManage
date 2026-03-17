cmake_minimum_required(VERSION 3.16)
project(CppTrendingStudentManage LANGUAGES CXX)

# 使用 C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 源文件
add_executable(StudentManage
    Student.cpp
    Student.h
)

# ---- 编译器编码设置：强制源码按 UTF-8 解释 ----
# MSVC（Visual Studio / MSVC 编译器）
target_compile_options(StudentManage PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/utf-8>
)

# MinGW / Clang on Windows（如果你用的是这些，可以一起开）
# target_compile_options(StudentManage PRIVATE
#     $<$<CXX_COMPILER_ID:GNU,Clang>:-finput-charset=UTF-8 -fexec-charset=UTF-8>
# )