cmake_minimum_required(VERSION 3.16)
project(main)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -fdiagnostics-color=always")

set(SOURCES
    main.cpp
    include/glad/glad.c
)

include_directories(
    /Users/noahdujovny/Documents/EECS/EECS498-007/3d_test/include
    /Users/noahdujovny/Documents/EECS/EECS498-007/3d_test/lib
)
link_directories(
    /Users/noahdujovny/Documents/EECS/EECS498-007/3d_test/lib
    /Users/noahdujovny/Documents/EECS/EECS498-007/3d_test/lib/SDL2
    /Users/noahdujovny/Documents/EECS/EECS498-007/3d_test/lib/SDL2_Mixer
    /Users/noahdujovny/Documents/EECS/EECS498-007/3d_test/lib/SDL2_ttf
)

add_executable(app ${SOURCES})

# Link GLFW
find_library(GLFW_LIBRARY NAMES libglfw.3.3.dylib PATHS /Users/noahdujovny/Documents/EECS/EECS498-007/3d_test/lib)
target_link_libraries(app ${GLFW_LIBRARY})

# Link SDL2 libraries
find_library(SDL2_LIBRARY NAMES SDL2 PATHS /Users/noahdujovny/Documents/EECS/EECS498-007/3d_test/lib/SDL2/lib)
find_library(SDL2_MIXER_LIBRARY NAMES SDL2_mixer PATHS /Users/noahdujovny/Documents/EECS/EECS498-007/3d_test/lib/SDL2_Mixer/lib)
find_library(SDL2_TTF_LIBRARY NAMES SDL2_ttf PATHS /Users/noahdujovny/Documents/EECS/EECS498-007/3d_test/lib/SDL2_ttf/lib)

# Add SDL libraries to the target
target_link_libraries(app 
    ${SDL2_LIBRARY}
    ${SDL2_MIXER_LIBRARY}
    ${SDL2_TTF_LIBRARY}
)


find_package(OpenGL REQUIRED)
target_link_libraries(app ${OPENGL_gl_LIBRARY})

if(APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    find_library(COREFOUNDATION_LIBRARY CoreFoundation)
    target_link_libraries(app ${COCOA_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY} ${COREFOUNDATION_LIBRARY})
endif()
