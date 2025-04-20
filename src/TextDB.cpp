//
//  TextDB.cpp
//  game_engine
//
//  Created by Noah Dujovny on 2/4/25.
//

#include <stdio.h>
#include "TextDB.h"

SDL_Renderer* TextDB::renderer = nullptr;
std::unordered_map<std::string, std::unordered_map<int, TTF_Font*>> TextDB::fonts;
std::queue<TextDB::TextDrawStruct> TextDB::drawQueue;

void TextDB::Init(SDL_Renderer* r) {
    if (TTF_Init() == -1) {
        std::cout << "Failed to initialize TTF: " << TTF_GetError() << std::endl;
        exit(1);
    }
    renderer = r;
}

void TextDB::Shutdown() {
    while (!drawQueue.empty()) {
        drawQueue.pop();
    }
    for (auto& font_family : fonts) {
        for (auto& font_pair : font_family.second) {
            TTF_CloseFont(font_pair.second);
        }
    }
    TTF_Quit();
}

void TextDB::LoadFont(const std::string& font_name, const std::string& file_path, int font_size) {
  if(!std::filesystem::exists(file_path)) {
    std::cout << "error: font " << font_name << " missing";
    exit(0);
  }
  
  TTF_Font* font = TTF_OpenFont(file_path.c_str(), font_size);
  if(!font) {
    std::cout << "Font " << font_name << " not loaded correctly" << std::endl;
    exit(0);
  }

  fonts[font_name][font_size] = font;
}

void TextDB::Draw(const std::string& str_content, float x, float y, const std::string& font_name, int font_size, float r, float g, float b, float a) {
    TextDrawStruct request = { str_content, x, y, font_name, font_size, r, g, b, a };
    drawQueue.push(request);
}

void TextDB::RenderText() {
    while (!drawQueue.empty()) {
        TextDrawStruct request = drawQueue.front();
        drawQueue.pop();

        if(request.str_content.empty()) {
            return;
        }
    
        if (fonts.find(request.font_name) == fonts.end() || fonts[request.font_name].find(request.font_size) == fonts[request.font_name].end()) {
            std::string fp = "resources/fonts/" + request.font_name + ".ttf";
            LoadFont(request.font_name, fp, request.font_size);
        }

        TTF_Font* font = fonts[request.font_name][request.font_size];
        SDL_Color color = { static_cast<Uint8>(request.r), static_cast<Uint8>(request.g), static_cast<Uint8>(request.b), static_cast<Uint8>(request.a) };

        SDL_Surface* surface = TTF_RenderText_Solid(font, request.str_content.c_str(), color);
        if (!surface) {
            std::cout << "Failed to create text surface: " << TTF_GetError() << std::endl;
            return;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) {
            std::cout << "Failed to create text texture: " << SDL_GetError() << std::endl;
            SDL_FreeSurface(surface);
            return;
        }

        SDL_Rect dstRect = { 
            static_cast<int>(request.x), 
            static_cast<int>(request.y), 
            surface->w, 
            surface->h 
        };

        SDL_RenderCopy(renderer, texture, NULL, &dstRect);

        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
}
