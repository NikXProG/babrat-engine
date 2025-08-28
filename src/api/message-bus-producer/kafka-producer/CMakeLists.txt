cmake_minimum_required(VERSION 3.6)
project(bbrt_srvr_src_api_brkr_prdsr-kafka)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
        librdkafka
        GIT_REPOSITORY https://github.com/edenhill/librdkafka.git
        GIT_TAG v2.2.0
)



# Оптимизация сборки
set(WITH_ZSTD OFF CACHE BOOL "" FORCE)
set(WITH_ZLIB OFF CACHE BOOL "" FORCE)
set(WITH_CURL OFF CACHE BOOL "" FORCE)
set(RDKAFKA_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(RDKAFKA_BUILD_TESTS OFF CACHE BOOL "" FORCE)

# 2. Загружаем modern-cppkafka
FetchContent_Declare(
        modern-cppkafka
        GIT_REPOSITORY https://github.com/morganstanley/modern-cpp-kafka.git
        GIT_TAG main  # Или конкретный коммит
)

# 3. Получаем зависимости
FetchContent_MakeAvailable(librdkafka modern-cppkafka)

# 4. Создаем алиас для modern-cppkafka (так как он не предоставляет готовую цель)
if(NOT TARGET modern-cpp-kafka::modern-cpp-kafka)
    add_library(modern-cpp-kafka::modern-cpp-kafka INTERFACE IMPORTED)
    target_include_directories(modern-cpp-kafka::modern-cpp-kafka INTERFACE
            ${modern-cppkafka_SOURCE_DIR}/include
    )
    target_link_libraries(modern-cpp-kafka::modern-cpp-kafka INTERFACE
            rdkafka
    )
endif()

# 5. Создаем исполняемый файл
add_executable(
        bbrt_srvr_src_api_brkr_prdsr-kafka
        kafka-consumer.cpp
)

# 6. Линкуем зависимости
target_link_libraries(
        bbrt_srvr_src_api_brkr_prdsr-kafka
        PRIVATE
        modern-cpp-kafka::modern-cpp-kafka

)

# 7. Для MacOS
if(APPLE)
    find_library(SECURITY_LIB Security)
    find_library(CORESERVICES_LIB CoreServices)
    target_link_libraries(bbrt_srvr_src_api_brkr_prdsr-kafka PRIVATE
            ${SECURITY_LIB}
            ${CORESERVICES_LIB}
    )
endif()