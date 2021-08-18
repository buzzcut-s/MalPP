cmake_minimum_required(VERSION 3.12.0)
project(
  MalPP
  VERSION 0.1.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_FLAGS_DEBUG
    "${CMAKE_C_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address,leak,undefined"
)
set(CMAKE_CXX_FLAGS_DEBUG
    "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address,leak,undefined"
)
set(CMAKE_LINKER_FLAGS_DEBUG
    "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address,leak,undefined"
)

add_executable(
  MalPP
  src/main.cpp
  include/mal.hpp
  src/mal.cpp
  include/readline.hpp
  src/readline.cpp
  include/Reader.hpp
  src/Reader.cpp
  include/printer.hpp
  src/printer.cpp
  include/types.hpp
  src/types.cpp
  include/Environment.hpp
  src/Environment.cpp
  include/eval.hpp
  src/eval.cpp)
