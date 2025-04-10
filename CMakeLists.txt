cmake_minimum_required(VERSION 3.16)
project(main)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fdiagnostics-color=always")

# Set Debug as default build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)

# Use file(GLOB...) to find all source files
file(GLOB_RECURSE SRC_FILES 
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

# If LuaBridge has .cpp files that need to be compiled
file(GLOB_RECURSE LUABRIDGE_SRC_FILES 
    "${CMAKE_SOURCE_DIR}/include/vendor/Lua/*.c"
)

# Define sources list
set(SOURCES
    ${SRC_FILES}
    ${LUABRIDGE_SRC_FILES}
    ${CMAKE_SOURCE_DIR}/include/vendor/glad/glad.c
)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/vendor
    ${CMAKE_SOURCE_DIR}/lib
    ${CMAKE_SOURCE_DIR}/lib/SDL2
    ${CMAKE_SOURCE_DIR}/lib/SDL2_Mixer
    ${CMAKE_SOURCE_DIR}/lib/SDL2_ttf
)
link_directories(
    ${CMAKE_SOURCE_DIR}/lib/glfw
    ${CMAKE_SOURCE_DIR}/lib/SDL2
    ${CMAKE_SOURCE_DIR}/lib/SDL2_Mixer
    ${CMAKE_SOURCE_DIR}/lib/SDL2_ttf
)

if(APPLE)
    # macOS-specific configuration

    add_executable(app ${SOURCES})

    # Link GLFW
    find_library(GLFW_LIBRARY NAMES libglfw.3.3.dylib PATHS ${CMAKE_SOURCE_DIR}/lib/glfw)
    target_link_libraries(app ${GLFW_LIBRARY})

    # Link SDL2 libraries
    find_library(SDL2_LIBRARY NAMES SDL2 PATHS ${CMAKE_SOURCE_DIR}/lib/SDL2/lib)
    find_library(SDL2_MIXER_LIBRARY NAMES SDL2_mixer PATHS ${CMAKE_SOURCE_DIR}/lib/SDL2_Mixer/lib)
    find_library(SDL2_TTF_LIBRARY NAMES SDL2_ttf PATHS ${CMAKE_SOURCE_DIR}/lib/SDL2_ttf/lib)

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
    add_executable(app ${SOURCES})

    # Link GLFW
    find_library(GLFW_LIBRARY NAMES glfw3.lib PATHS ${CMAKE_SOURCE_DIR}/lib/glfw)
    target_link_libraries(app ${GLFW_LIBRARY})

    # Link SDL2 libraries
    find_library(SDL2_LIBRARY NAMES SDL2.lib PATHS ${CMAKE_SOURCE_DIR}/lib/SDL2/lib)
    find_library(SDL2_MAIN_LIBRARY NAMES SDL2main.lib PATHS ${CMAKE_SOURCE_DIR}/lib/SDL2/lib)
    find_library(SDL2_MIXER_LIBRARY NAMES SDL2_mixer.lib PATHS ${CMAKE_SOURCE_DIR}/lib/SDL2_Mixer/lib)
    find_library(SDL2_TTF_LIBRARY NAMES SDL2_ttf.lib PATHS ${CMAKE_SOURCE_DIR}/lib/SDL2_ttf/lib)

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
