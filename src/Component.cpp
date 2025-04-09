#include "Component.h"
#include "ComponentManager.h"

bool Component::IsEnabled() {
    if(type == "ParticleSystem") {
        return enabled;
    }else if (componentRef && componentRef->isTable()) {
        if ((*componentRef)["enabled"].isBool()) {
            enabled = (*componentRef)["enabled"].cast<bool>();
        }
    } else if (type == "Rigidbody" || type == "ParticleSystem") {
        enabled = true;
    }
    return enabled;
}


void Component::SetComponentProps() {
    if (componentRef->isTable()) {
        if ((*componentRef)["OnStart"].isFunction()) {
            hasStart = true;
        }
        if ((*componentRef)["OnUpdate"].isFunction()) {
            hasUpdate = true;
        }
        if ((*componentRef)["OnLateUpdate"].isFunction()) {
            hasLateUpdate = true;
        }
        if ((*componentRef)["OnDestroy"].isFunction()) {
            hasDestroy = true;
        }
    }
}