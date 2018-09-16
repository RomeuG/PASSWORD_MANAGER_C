cmake_minimum_required(VERSION 3.11)
project(PASSWORD_MANAGER)

set(CMAKE_C_STANDARD 11)

include_directories(src)
set(CMAKE_EXE_LINKER_FLAGS "-lsqlite3 -lssl -lcrypto")

set(CMAKE_C_FLAGS_DEBUG "-ggdb3 -O0")

add_definitions(-DDEBUG -D_GNU_SOURCE)

# profiler flags and verification
set(PROFILER FALSE)
if(PROFILER)

endif()

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
        README.md src/tests.h src/tests.c)
