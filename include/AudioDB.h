//
//  AudioDB.h
//  game_engine
//
//  Created by Noah Dujovny on 1/28/25.
//
#ifndef AUDIODB_H
#define AUDIODB_H

#include <string>
#include <unordered_map>
#include "SDL2_Mixer/SDL_mixer.h"

class AudioDB {
public:
    static void Init();
    static void Shutdown();
  
    static void LoadAudio(const std::string& file_name);
    static int PlayChannel(int channel, const std::string& audio_name, bool loops);
    static void HaltChannel(int channel);
    static void SetVolume(int channel, float volume);
private:
    static std::unordered_map<std::string, Mix_Chunk*> audio_chunks;
};

#endif
