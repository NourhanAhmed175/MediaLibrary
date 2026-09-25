cmake_minimum_required(VERSION 3.10)
project(MediaLibrary VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Warnings required by the project rules: -Wall -Wextra with zero warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra)
endif()

# All the logic lives in one static library so that both executables share it
add_library(medialib STATIC
    src/Duration.cpp
    src/Media.cpp
    src/Song.cpp
    src/Video.cpp
    src/Podcast.cpp
    src/MediaUtils.cpp
    src/Library.cpp
    src/Playlist.cpp
    src/PlaylistManager.cpp
    src/SampleData.cpp
)
target_include_directories(medialib PUBLIC include)

# Interactive console menu
add_executable(media_app src/main.cpp)
target_link_libraries(media_app PRIVATE medialib)

# Demo driver: runs every feature end to end, no input needed
add_executable(demo src/demo.cpp)
target_link_libraries(demo PRIVATE medialib)
