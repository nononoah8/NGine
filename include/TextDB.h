//
//  TextDB.h
//  game_engine
//
//  Created by Noah Dujovny on 2/4/25.
//

#ifndef TextDB_h
#define TextDB_h

#include <string>
#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <queue>
#include "SDL2/SDL.h"
#include "SDL2_ttf/SDL_ttf.h"

class TextDB {
public:
    static void Init(SDL_Renderer* renderer);
    static void Shutdown();

    static void LoadFont(const std::string& font_name, const std::string& file_path, int font_size);
    static void Draw(const std::string& str_content, float x, float y, const std::string& font_name, int font_size, float r, float g, float b, float a);
    static void RenderText();
private:
    struct TextDrawStruct {
        std::string str_content;
        float x, y;
        std::string font_name;
        int font_size;
        float r, g, b, a;
    };

    static SDL_Renderer* renderer;
    static std::unordered_map<std::string, std::unordered_map<int, TTF_Font*>> fonts;
    static std::queue<TextDrawStruct> drawQueue;
};

#endif /* TextDB_h */
