cmake_minimum_required(VERSION 3.10)

project(DistributedKVStoreCpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(kv_store
    main.cpp
    src/KVStore.cpp
    src/CommandParser.cpp
    src/WAL.cpp
)

add_executable(kv_server
    server_main.cpp
    src/KVStore.cpp
    src/CommandParser.cpp
    src/WAL.cpp
)