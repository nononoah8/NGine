#ifndef EVENTSYSTEM_HPP
#define EVENTSYSTEM_HPP

#include "include/Lua/lua.hpp"
#include "include/LuaBridge/LuaBridge.h"

#include <unordered_map>
#include <vector>
#include <string>

struct PendingChange {
    std::string event_type;
    luabridge::LuaRef component;
    luabridge::LuaRef function;
    bool subscribed;

    PendingChange(const std::string& event_type, luabridge::LuaRef component, luabridge::LuaRef function, bool subscribed)
    : event_type(event_type), component(component), function(function), subscribed(subscribed) {}
};

class EventSystem {
public:
    static void Subscribe(const std::string& event_type, luabridge::LuaRef component, luabridge::LuaRef function);
    static void Unsubscribe(const std::string& event_type, luabridge::LuaRef component, luabridge::LuaRef function);
    static void Publish(const std::string& event_type, luabridge::LuaRef event_object);
    static void ProcessPendingChanges();
private:
    static std::unordered_map<std::string, std::vector<std::pair<luabridge::LuaRef, luabridge::LuaRef>>> subscriptions;
    static std::vector<PendingChange> pendingChanges;
};

#endif
