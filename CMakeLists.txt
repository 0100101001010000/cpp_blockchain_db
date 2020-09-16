cmake_minimum_required(VERSION 3.17)
project(cpp_blockchain_db)

set(CMAKE_CXX_STANDARD 20)

add_executable(cpp_blockchain_db main.cpp unique_id.cpp unique_id.h block.cpp block.h chain.cpp chain.h picosha2.h)