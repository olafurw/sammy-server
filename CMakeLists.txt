cmake_minimum_required(VERSION 2.6)
project(game_server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -O0") 

set(LIBRARY_FILES
    libraries/format.cc
)

set(SERVER_FILES
    utils.cpp
    sha256.cpp
    templates.cpp
    cache_storage.cpp
    config.cpp
    database.cpp
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
    soci_core
    soci_mysql
    /usr/lib64/mysql/libmysqlclient.so
)

target_link_libraries(server ${LIBRARIES})