cmake_minimum_required(VERSION 3.17)
project(cpp_blockchain_db)

set(CMAKE_CXX_STANDARD 20)

add_library(cpp_blockchain_db unique_id.cpp unique_id.h block.cpp block.h chain.cpp chain.h picosha2.h)
add_executable(cpp_blockchain_tests tests.cpp unique_id.cpp unique_id.h block.cpp block.h chain.cpp chain.h picosha2.h)

enable_testing()
add_test(cpp_blockchain_tests cpp_blockchain_tests)