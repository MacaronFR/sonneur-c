cmake_minimum_required(VERSION 4.0)
project(sonneur C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(sonneur_test main.c src/descriptor.c src/serializer.c)

target_link_directories(sonneur_test PUBLIC lib)

target_link_libraries(sonneur_test sonneur)

add_library(sonneur
        src/descriptor.c
        src/serializer.c
        src/hash.c
        src/deserializer.c)
