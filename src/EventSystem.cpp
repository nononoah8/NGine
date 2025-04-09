#include "EventSystem.h"

#include <algorithm>

// Define the static member variables
std::unordered_map<std::string, std::vector<std::pair<luabridge::LuaRef, luabridge::LuaRef>>> EventSystem::subscriptions;
std::vector<PendingChange> EventSystem::pendingChanges;

void EventSystem::Subscribe(const std::string& event_type, luabridge::LuaRef component, luabridge::LuaRef function) {
    PendingChange pendingChange{ event_type, component, function, true };
    pendingChanges.push_back(pendingChange);
}

void EventSystem::Unsubscribe(const std::string& event_type, luabridge::LuaRef component, luabridge::LuaRef function) {
    PendingChange pendingChange{ event_type, component, function, false };
    pendingChanges.push_back(pendingChange);
}

void EventSystem::Publish(const std::string& event_type, luabridge::LuaRef event_object) {
    auto it = subscriptions.find(event_type);
    if(it != subscriptions.end()) {
        // Loop thru the subscriptions
        for(auto& subscriber : it->second) {
            // Get the component and function reference
            const auto& component = subscriber.first;
            const auto& function = subscriber.second;

            try {
                function(component, event_object);
            } catch(const luabridge::LuaException& e) {
                std::cout << "Error with subscribed function: " << e.what() << std::endl;
            }
        }
    }
}

void EventSystem::ProcessPendingChanges() {
    for(auto change : pendingChanges) {
        const std::string& event_type = change.event_type;
        const luabridge::LuaRef& component = change.component;
        const luabridge::LuaRef& function = change.function;
        bool subscribed = change.subscribed;

        if(subscribed) {
            subscriptions[event_type].push_back({ component, function });
        }else {
            auto& subscriber_list = subscriptions[event_type];
            subscriber_list.erase(
                std::remove_if(subscriber_list.begin(), subscriber_list.end(),
                    [&](const std::pair<luabridge::LuaRef, luabridge::LuaRef>& subscriber) {
                        return subscriber.first == component && subscriber.second == function;
                    }),
                subscriber_list.end());
        }
    }
    pendingChanges.clear();
}