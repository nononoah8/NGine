//
//  ReadJSON.h
//  game_engine
//
//  Created by Noah Dujovny on 1/29/25.
//

#ifndef ReadJSON_h
#define ReadJSON_h

#include <iostream>
#include <string>
#include <vector>
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

static void ReadJsonFile(const std::string& path, rapidjson::Document & out_document)
{
    FILE* file_pointer = nullptr;
#ifdef _WIN32
    fopen_s(&file_pointer, path.c_str(), "rb");
#else
    file_pointer = fopen(path.c_str(), "rb");
#endif
    char buffer[65536];
    rapidjson::FileReadStream stream(file_pointer, buffer, sizeof(buffer));
    out_document.ParseStream(stream);
    std::fclose(file_pointer);
    
    if (out_document.HasParseError()) {
        rapidjson::ParseErrorCode errorCode = out_document.GetParseError();
        std::cout << "error parsing json at [" << path << "]" << std::endl;
        exit(0);
    }
}

static std::string getJsonStringOrDefault(const rapidjson::Document& doc, const std::string& key, const std::string& defaultVal) {
    if (doc.HasMember(key.c_str()) && doc[key.c_str()].IsString()) {
        return doc[key.c_str()].GetString();
    }
    return defaultVal;
}

static int getJsonIntOrDefault(const rapidjson::Document& doc, const std::string& key, const int& defaultVal) {
    if (doc.HasMember(key.c_str()) && doc[key.c_str()].IsInt()) {
        return doc[key.c_str()].GetInt();
    }
    return defaultVal;
}

static float getJsonFloatOrDefault(const rapidjson::Document& doc, const std::string& key, const float& defaultVal) {
    if (doc.HasMember(key.c_str()) && (doc[key.c_str()].IsFloat() || doc[key.c_str()].IsInt())) {
        return doc[key.c_str()].GetFloat();
    }
    return defaultVal;
}

static std::vector<std::string> getJsonArrayOrDefault(const rapidjson::Document& doc, const std::string& key, const std::vector<std::string>& defaultVal) {
    if (doc.HasMember(key.c_str()) && doc[key.c_str()].IsArray()) {
        std::vector<std::string> result;
        result.reserve(doc[key.c_str()].Size());
        for (auto& v : doc[key.c_str()].GetArray()) {
            if (v.IsString()) {
                result.push_back(v.GetString());
            }
        }
        return result;
    }
    return defaultVal;
}

static bool getJsonBoolOrDefault(const rapidjson::Document& doc, const std::string& key, const bool defaultVal) {
    if (doc.HasMember(key.c_str()) && doc[key.c_str()].IsBool()) {
        return doc[key.c_str()].GetBool();
    }
    return defaultVal;
}

// Overloads for rapidjson::Value
static std::string getJsonStringOrDefault(const rapidjson::Value& value, const std::string& key, const std::string& defaultVal) {
    if (value.HasMember(key.c_str()) && value[key.c_str()].IsString()) {
        return value[key.c_str()].GetString();
    }
    return defaultVal;
}

static int getJsonIntOrDefault(const rapidjson::Value& value, const std::string& key, const int& defaultVal) {
    if (value.HasMember(key.c_str()) && value[key.c_str()].IsInt()) {
        return value[key.c_str()].GetInt();
    }
    return defaultVal;
}

static float getJsonFloatOrDefault(const rapidjson::Value& value, const std::string& key, const float& defaultVal) {
    if (value.HasMember(key.c_str()) && (value[key.c_str()].IsFloat() || value[key.c_str()].IsInt())) {
        return value[key.c_str()].GetFloat();
    }
    return defaultVal;
}

static std::vector<std::string> getJsonArrayOrDefault(const rapidjson::Value& value, const std::string& key, const std::vector<std::string>& defaultVal) {
    if (value.HasMember(key.c_str()) && value[key.c_str()].IsArray()) {
        std::vector<std::string> result;
        result.reserve(value[key.c_str()].Size());
        for (auto& v : value[key.c_str()].GetArray()) {
            if (v.IsString()) {
                result.push_back(v.GetString());
            }
        }
        return result;
    }
    return defaultVal;
}

static bool getJsonBoolOrDefault(const rapidjson::Value& value, const std::string& key, const bool defaultVal) {
    if (value.HasMember(key.c_str()) && value[key.c_str()].IsBool()) {
        return value[key.c_str()].GetBool();
    }
    return defaultVal;
}

#endif /* ReadJSON_h */
