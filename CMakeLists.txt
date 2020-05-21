cmake_minimum_required(VERSION 3.16.3 FATAL_ERROR)

project (usop VERSION 0.0.1 LANGUAGES CXX DESCRIPTION "Unsynchronized Object Pool")

set (CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set (CMAKE_CXX_EXTENSIONS False)

add_library (project_options INTERFACE)
target_compile_features (project_options INTERFACE cxx_std_11)

add_library (project_warnings INTERFACE)
target_compile_options (project_warnings INTERFACE -Wall -Wextra -Werror -pedantic)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
   target_compile_options (project_warnings INTERFACE -Weverything
      -Wno-c++98-compat -Wno-c++98-compat-pedantic
      -Wno-weak-vtables -Wno-global-constructors -Wno-padded)
endif ()

include (${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup (TARGETS NO_OUTPUT_DIRS SKIP_STD)

add_library (sbit INTERFACE)
target_include_directories (sbit INTERFACE include)
target_compile_features (sbit INTERFACE cxx_std_11)

include (GoogleTest)
enable_testing ()

add_subdirectory (test)
