cmake_minimum_required(VERSION 3.1)
project(dashDrive)

set (CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS "-D_FILE_OFFSET_BITS=64")

include_directories(/usr/local/include)

find_library(FUSE osxfuse)
find_path(FUSE_INCLUDE fuse/fuse.h
          HINTS /usr/local/include/osxfuse/)
include_directories(${FUSE_INCLUDE})

add_executable(fusedrive fusedrive.cpp)
target_link_libraries(fusedrive ${CPP_REST} ${FUSE})
