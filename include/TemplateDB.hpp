//
//  TemplateDB.hpp
//  game_engine
//
//  Created by Noah Dujovny on 1/29/25.
//

#ifndef TemplateDB_hpp
#define TemplateDB_hpp

#include "Component.h"
#include <string>
#include <iostream>
#include <unordered_map>

class ActorTemplate {
public:
    ActorTemplate(std::string name) : name(name) {}

    std::string name;
    std::map<std::string, std::shared_ptr<Component>> components;
};

#endif
