cmake_minimum_required(VERSION 3.21.0)
project(
  MalPP
  VERSION 1.0.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(
  MalPP
  src/main.cpp
  src/mal.cpp
  src/readline.cpp
  src/Reader.cpp
  src/printer.cpp
  src/types.cpp
  src/Environment.cpp
  src/eval.cpp
  src/core.cpp
  src/Data.cpp
  src/types/False.cpp
  src/types/Function.cpp
  src/types/HashMap.cpp
  src/types/Integer.cpp
  src/types/Keyword.cpp
  src/types/List.cpp
  src/types/Nil.cpp
  src/types/String.cpp
  src/types/Symbol.cpp
  src/types/True.cpp
  src/types/Vector.cpp)
