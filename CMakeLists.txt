cmake_minimum_required(VERSION 3.16)
project(main)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -fdiagnostics-color=always")

set(SOURCES
    main.cpp
    include/glad/glad.c
)

if(APPLE)
    # macOS-specific configuration
    include_directories(
        /Users/noahdujovny/Documents/EECS/EECS498-007/3d_test/include
        /Users/noahdujovny/Documents/EECS/EECS498-007/3d_test/lib
    )
    link_directories(
        /Users/noahdujovny/Documents/EECS/EECS498-007/3d_test/lib/glfw
        /Users/noahdujovny/Documents/EECS/EECS498-007/3d_test/lib/SDL2
        /Users/noahdujovny/Documents/EECS/EECS498-007/3d_test/lib/SDL2_Mixer
        /Users/noahdujovny/Documents/EECS/EECS498-007/3d_test/lib/SDL2_ttf
    )

    add_executable(app ${SOURCES})

    # Link GLFW
    find_library(GLFW_LIBRARY NAMES libglfw.3.3.dylib PATHS /Users/noahdujovny/Documents/EECS/EECS498-007/3d_test/lib/glfw)
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

    # Link OpenGL
    find_package(OpenGL REQUIRED)
    target_link_libraries(app ${OPENGL_gl_LIBRARY})

    # macOS-specific frameworks
    find_library(COCOA_LIBRARY Cocoa)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    find_library(COREFOUNDATION_LIBRARY CoreFoundation)
    target_link_libraries(app ${COCOA_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY} ${COREFOUNDATION_LIBRARY})

elseif(WIN32)
    # Windows-specific configuration
    include_directories(
        C:/Users/noahd/Documents/EECS/EECS498-007/3d_test/include
        C:/Users/noahd/Documents/EECS/EECS498-007/3d_test/lib
    )
    link_directories(
        C:/Users/noahd/Documents/EECS/EECS498-007/3d_test/lib/glfw
        C:/Users/noahd/Documents/EECS/EECS498-007/3d_test/lib/SDL2
        C:/Users/noahd/Documents/EECS/EECS498-007/3d_test/lib/SDL2_Mixer
        C:/Users/noahd/Documents/EECS/EECS498-007/3d_test/lib/SDL2_ttf
    )

    add_executable(app ${SOURCES})

    # Link GLFW
    find_library(GLFW_LIBRARY NAMES glfw3.lib PATHS C:/Users/noahd/Documents/EECS/EECS498-007/3d_test/lib/glfw)
    target_link_libraries(app ${GLFW_LIBRARY})

    # Link SDL2 libraries
    find_library(SDL2_LIBRARY NAMES SDL2.lib PATHS C:/Users/noahd/Documents/EECS/EECS498-007/3d_test/lib/SDL2/lib)
    find_library(SDL2_MAIN_LIBRARY NAMES SDL2main.lib PATHS C:/Users/noahd/Documents/EECS/EECS498-007/3d_test/lib/SDL2/lib)
    find_library(SDL2_MIXER_LIBRARY NAMES SDL2_mixer.lib PATHS C:/Users/noahd/Documents/EECS/EECS498-007/3d_test/lib/SDL2_Mixer/lib)
    find_library(SDL2_TTF_LIBRARY NAMES SDL2_ttf.lib PATHS C:/Users/noahd/Documents/EECS/EECS498-007/3d_test/lib/SDL2_ttf/lib)

    # Add SDL libraries to the target
    target_link_libraries(app 
        ${SDL2_LIBRARY}
        ${SDL2_MAIN_LIBRARY}
        ${SDL2_MIXER_LIBRARY}
        ${SDL2_TTF_LIBRARY}
    )

    # Link OpenGL
    find_package(OpenGL REQUIRED)
    target_link_libraries(app ${OPENGL_gl_LIBRARY})

    # Copy DLLs to the output directory
    set(DLLS
        ${CMAKE_SOURCE_DIR}/dlls/glfw3.dll
        ${CMAKE_SOURCE_DIR}/dlls/SDL2.dll
        ${CMAKE_SOURCE_DIR}/dlls/SDL2_mixer.dll
        ${CMAKE_SOURCE_DIR}/dlls/SDL2_ttf.dll
    )

    foreach(DLL ${DLLS})
        add_custom_command(TARGET app POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${DLL}
            $<TARGET_FILE_DIR:app>
        )
    endforeach()
endif()