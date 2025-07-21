cmake_minimum_required(VERSION 3.6)
project(bbrt_srvr_src_api_brkr_consmr)

add_subdirectory(kafka-consumer)

#[[
set_target_properties(
        bbrt_srvr_src_api_brkr_consmr PROPERTIES
        LANGUAGES CXX
        LINKER_LANGUAGE CXX
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        VERSION 1.0
        DESCRIPTION "broker consumer classes library")]]
