#include "Entity/EntityLogic.hpp"
#include "Entity.hpp"

#include <cassert>

EntityLogic::EntityLogic() :
    entity(nullptr) {
}

EntityLogic::~EntityLogic() {
}

void EntityLogic::setEntity(const Entity* ownEntity) {
    entity = ownEntity;
}

EntityId EntityLogic::getEntityId() const {
    if(entity) {
        return entity->getEntityId();
    }
    assert(false && "No valid entity to query Id");
    return InvalidEntityId;
}

EntityId EntityLogic::getParentId() const {
    if(entity) {
        return entity->ET_getParentId();
    }
    assert(false && "No valid entity to query parent's Id");
    return InvalidEntityId;
}

const char* EntityLogic::getEntityName() const {
    if(entity) {
        return entity->ET_getName();
    }
    assert(false && "No valid entity to query name");
    return "";
}