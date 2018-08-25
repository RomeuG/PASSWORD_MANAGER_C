cmake_minimum_required(VERSION 3.11)
project(PASSWORD_MANAGER)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)
set(CMAKE_EXE_LINKER_FLAGS "-lsqlite3 -lssl -lcrypto")

add_definitions(-DDEBUG -D_GNU_SOURCE)

add_executable(PASSWORD_MANAGER
        src/base64.c
        src/base64.h
        src/database.c
        src/database.h
        src/defines.h
        src/encryption.c
        src/encryption.h
        src/main.c
        src/utils.c
        src/utils.h
        README.md)
