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
  src/eval.cpp
  include/core.hpp
  src/core.cpp
  include/Data.hpp
  src/Data.cpp
  include/types/False.hpp
  include/types/Function.hpp
  include/types/HashMap.hpp
  include/types/Integer.hpp
  include/types/Keyword.hpp
  include/types/List.hpp
  include/types/Nil.hpp
  include/types/String.hpp
  include/types/Symbol.hpp
  include/types/True.hpp
  include/types/Vector.hpp
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
