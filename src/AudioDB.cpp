//
//  AudioDB.cpp
//  game_engine
//
//  Created by Noah Dujovny on 2/5/25.
//

#include "SDL2_Mixer/SDL_mixer.h"
#include "AudioDB.h"
#include <iostream>
#include <filesystem>

std::unordered_map<std::string, Mix_Chunk*> AudioDB::audio_chunks;

void AudioDB::Init() {
  if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    std::cout << "SDL_mixer could not initialize: " << Mix_GetError() << std::endl;
    exit(1);
  }
  Mix_AllocateChannels(50);
}

void AudioDB::Shutdown() {
    for (auto& chunk : audio_chunks) {
        Mix_FreeChunk(chunk.second);
    }
    audio_chunks.clear();

    Mix_CloseAudio();
}

void AudioDB::LoadAudio(const std::string& audio_name) {
    std::string wav_path = "resources/audio/" + audio_name + ".wav";
    std::string ogg_path = "resources/audio/" + audio_name + ".ogg";
    std::string filename = "";

    if (std::filesystem::exists(wav_path)) {
        filename = wav_path;
    } else if (std::filesystem::exists(ogg_path)) {
        filename = ogg_path;
    } else {
        std::cout << "error: failed to play audio clip " << audio_name;
        exit(0);
    }

    Mix_Chunk* chunk = Mix_LoadWAV(filename.c_str());

    audio_chunks[audio_name] = chunk;
}

int AudioDB::PlayChannel(int channel, const std::string& audio_name, bool loops) {
    if (audio_chunks.find(audio_name) == audio_chunks.end()) {
        LoadAudio(audio_name);
    }
    return Mix_PlayChannel(channel, audio_chunks[audio_name], loops ? -1 : 0);
}

void AudioDB::SetVolume(int channel, float volume) {
    int vol = static_cast<int>(volume);
    if (vol < 0) vol = 0;
    if (vol > 128) vol = 128;
    Mix_Volume(channel, vol);
}

void AudioDB::HaltChannel(int channel) {
    Mix_HaltChannel(channel);
}
