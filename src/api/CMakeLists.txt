cmake_minimum_required(VERSION 3.30)
project(bbrt_srvr_src_api
        VERSION 1.0
        DESCRIPTION "here interactions take place through brokers or api"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(message-bus-consumer)

