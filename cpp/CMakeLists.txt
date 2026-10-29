cmake_minimum_required(VERSION 3.16)
project(lab3 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(COVERAGE "Собрать с инструментацией покрытия (gcov/lcov)" OFF)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)
if(COVERAGE)
  add_compile_options(--coverage -O0 -g)
  add_link_options(--coverage)
endif()

add_library(lr3 STATIC
  src/IContainer.cpp src/MyArray.cpp src/SinglyLinkedList.cpp src/DoublyLinkedList.cpp
  src/Stack.cpp src/HashTable.cpp src/CompleteBinaryTree.cpp)
target_include_directories(lr3 PUBLIC include)

add_executable(lab3 app/main.cpp)
target_link_libraries(lab3 lr3)

add_executable(lab3_bench bench/bench.cpp)
target_link_libraries(lab3_bench lr3)

find_package(GTest REQUIRED)
add_executable(lab3_tests tests/test_containers.cpp tests/test_serialization.cpp)
target_link_libraries(lab3_tests lr3 GTest::gtest_main)
target_compile_options(lab3_tests PRIVATE -Wno-variadic-macro-arguments-omitted -Wno-self-move -Wno-self-assign-overloaded)

enable_testing()
add_test(NAME unit COMMAND lab3_tests)

# macOS (Apple Silicon) может завершить свежесобранный файл с «Killed: 9», если подпись устарела — подписываем заново
if(APPLE)
  foreach(t lab3 lab3_bench lab3_tests)
    add_custom_command(TARGET ${t} POST_BUILD COMMAND codesign -f -s - $<TARGET_FILE:${t}> VERBATIM)
  endforeach()
endif()
