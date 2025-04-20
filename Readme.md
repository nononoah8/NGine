# 3D Engine

A modern, lightweight 3D rendering engine with component architecture and Lua scripting integration.

## Features

- Component-based architecture for flexible object behavior
- Lua scripting API for game logic and behavior
- 3D model loading and rendering
- Material and lighting support
- Camera controls with full transformation capabilities
- Input system (keyboard, mouse)
- Text rendering
- Audio playback system
- Scene management with actor creation and destruction
- Event system for decoupled communication

## Resource Structure

In order to create a game, you must have a resources folder in the root of the engine. It should be structured like this:

    ./resources
    ├── actor_templates
    │   └── Chair.template
    ├── audio
    │   └── GameMusic.mp3
    ├── component_types
    │   └── GameScript.lua
    ├── fonts
    │   └── BasicFont.ttf
    ├── models
    │   └── chair
    │       ├── chair.obj
    │       └── chair.mtl
    ├── scenes
    │    └── basic.scene
    ├── rendering.config
    └── game.config

All of your resources (models, audio, fonts, components, actor templates, and scenes) need to be stored in the respective folders for the engine to recognize them.

The actor templates, scenes, and config files are all JSON, the custom file extension helps the engine decide which is which.

## Config

The config files should just be lists of rendering or game variables.

Some important rendering variables include:
- `x_resolution`: the number of pixels across that your game window will be
- `y_resolution`: the number of pixels tall that your game window will be
- `debug`: set to true to have the engine draw the outlines of all the triangles
- `clear_color_r`: red value for the background color
- `clear_color_g`: green value for the background color
- `clear_color_b`: blue value for the background color


Some important game variables include:
- `game_title`: the name of your game
- `initial_scene`: the first scene that will be loaded when your game is opened

## Scenes

The scenes should be structured like so:

```json
{
    "actors": [
        {
            "name": "Chair",
            "components": {
                "2": {
                    "type": "ModelRenderer",
                    "model": "chair"
                },
                "3": {
                    "type": "ChairController"
                }
            }
        },
        {
            "name": "MainLight",
            "components": {
                "1": {
                    "type": "Transform"
                },
                "2": {
                    "type": "LightComponent",
                    "type": "directional",
                    "intensity": 1.5
                }
            }
        }
    ]
}
```

## Build Instructions

### Windows
```bash
cmake --build . Debug
start Debug/app.exe
```

### MacOS
```cmake .
make
./Debug/app
```

