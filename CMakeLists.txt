cmake_minimum_required(VERSION 2.6)
project(game_server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -O0") 

set(LIBRARY_FILES
    libraries/format.cc
    libraries/sha256.cpp
)

set(SERVER_FILES
    utils.cpp
    command_line.cpp
    storage_handler.cpp
    request_logger.cpp
    cache_storage.cpp
    blog_storage.cpp
    template_storage.cpp
    config.cpp
    listener.cpp
    router.cpp
    request_parser.cpp
    data_handler.cpp
    main.cpp
)

add_executable(server ${SERVER_FILES} ${LIBRARY_FILES})

set(LIBRARIES 
    pthread
    dl
)

target_link_libraries(server ${LIBRARIES})
