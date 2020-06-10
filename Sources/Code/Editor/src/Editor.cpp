#include "Editor.hpp"
#include "EditorApp.hpp"
#include "Core/Core.hpp"
#include "Core/MemoryStream.hpp"
#include "Core/Buffer.hpp"

EditorApp* EDITOR_APP = nullptr;
Buffer INTERNAL_BUFFER;

uint32_t Initiliaze() {
    if(EDITOR_APP) {
        return 1u;
    }
    EDITOR_APP = new EditorApp();
    if(!EDITOR_APP) {
        return 2u;
    }
    if(!EDITOR_APP->initialize()) {
        delete EDITOR_APP;
        EDITOR_APP = nullptr;
        return 3u;
    }
    return 0u;
}

const char* GetReflectModel() {
    if(!EDITOR_APP) {
        return nullptr;
    }
    INTERNAL_BUFFER = EDITOR_APP->getReflectModel();
    return INTERNAL_BUFFER.getCString();
}

uint32_t GetRegisteredEntityLogics(void** out) {
    if(!EDITOR_APP) {
        return 0u;
    }
    auto logicNames = EDITOR_APP->getRegisteredEntityLogics();
    if(logicNames.empty()) {
        return 0u;
    }
    INTERNAL_BUFFER.resize(sizeof(const char*) * logicNames.size());
    auto ptr = static_cast<const void**>(INTERNAL_BUFFER.getWriteData());
    for(size_t i = 0, sz = logicNames.size(); i < sz; ++i) {
        ptr[i] = static_cast<const void*>(logicNames[i]);
    }
    *out = INTERNAL_BUFFER.getWriteData();
    return static_cast<uint32_t>(logicNames.size());
}

void DeInitialize() {
    if(!EDITOR_APP) {
        return;
    }
    delete EDITOR_APP;
    EDITOR_APP = nullptr;
}

uint32_t LoadEntity(const char* entityName) {
    if(!EDITOR_APP) {
        return InvalidEntityId.getRawId();
    }
    auto entityId = EDITOR_APP->loadEntity(entityName);
    return entityId.getRawId();
}

void UnloadEntity(uint32_t entityId) {
    if(!EDITOR_APP) {
        return;
    }
    EntityId entId;
    entId.setRawId(entityId);
    EDITOR_APP->unloadEntity(entId);
}

uint32_t GetEntityChildEntityId(uint32_t entityId, int32_t childId) {
    if(!EDITOR_APP) {
        return 0u;
    }
    EntityId entId;
    entId.setRawId(entityId);
    auto childEntId = EDITOR_APP->getEntityChildEntityId(entId, static_cast<EntityChildId>(childId));
    return childEntId.getRawId();
}

const char* GetEntityName(uint32_t entityId) {
    if(!EDITOR_APP) {
        return nullptr;
    }
    EntityId entId;
    entId.setRawId(entityId);
    return EDITOR_APP->getEntityName(entId);
}

void DrawFrame(void* out, uint32_t w, uint32_t h) {
    if(!EDITOR_APP) {
        return;
    }
    EDITOR_APP->drawFrame(out, w, h);
}

int32_t AddLogicToEntity(uint32_t entityId, const char* logicName) {
    if(!EDITOR_APP) {
        return 0u;
    }
    EntityId entId;
    entId.setRawId(entityId);
    return static_cast<int32_t>(EDITOR_APP->addLogicToEntity(entId, logicName));
}

void RemoveLogicFromEntity(uint32_t entityId, int32_t logicId) {
    if(!EDITOR_APP) {
        return;
    }
    EntityId entId;
    entId.setRawId(entityId);
    EDITOR_APP->removeLogicFromEntity(entId, static_cast<EntityLogicId>(logicId));
}

int32_t AddChildEntityToEntity(uint32_t parentId, uint32_t childId) {
    if(!EDITOR_APP) {
        return 0u;
    }
    EntityId parentEntId;
    parentEntId.setRawId(parentId);
    EntityId childEntId;
    childEntId.setRawId(childId);
    return  EDITOR_APP->addChilEntityToEntity(parentEntId, childEntId);
}

void RemoveChildEntityFromEntity(uint32_t parentEntityId, uint32_t childEntityId) {
    if(!EDITOR_APP) {
        return;
    }
    EntityId parentEntId;
    parentEntId.setRawId(parentEntityId);
    EntityId childEntId;
    childEntId.setRawId(childEntityId);
    EDITOR_APP->removeChildEntityFromEntity(parentEntId, childEntId);
}

void AddEntityLogicArrayElement(uint32_t entityId, int32_t logicId, int32_t valueId) {
    if(!EDITOR_APP) {
        return;
    }
    EntityId entId;
    entId.setRawId(entityId);
    EDITOR_APP->addEntityLogicArrayElement(entId, static_cast<EntityLogicId>(logicId),
        static_cast<EntityLogicValueId>(valueId));
}

uint32_t GetEntityLogicData(uint32_t entityId, int32_t logicId, int32_t valueId, void** out) {
    if(!EDITOR_APP) {
        return 0u;
    }
    EntityId entId;
    entId.setRawId(entityId);
    auto buffer = EDITOR_APP->getEntityLogicData(entId, static_cast<EntityLogicId>(logicId),
        static_cast<EntityLogicValueId>(valueId));
    INTERNAL_BUFFER = std::move(buffer);
    *out = INTERNAL_BUFFER.getWriteData();
    return static_cast<uint32_t>(INTERNAL_BUFFER.getSize());
}

void SetEntityLogicData(uint32_t entityId, int32_t logicId, int32_t valueId, const void* data, uint32_t size) {
    if(!EDITOR_APP) {
        return;
    }
    EntityId entId;
    entId.setRawId(entityId);
    Buffer buffer(data, size);
    EDITOR_APP->setEntityLogicData(entId, static_cast<EntityLogicId>(logicId),
        static_cast<EntityLogicValueId>(valueId), buffer);
}

void UnloadAll() {
    if(!EDITOR_APP) {
        return;
    }
    EDITOR_APP->unloadAll();
}

int32_t CreateChildEntity(uint32_t entityId, const char* childName) {
    if(!EDITOR_APP) {
        return InvalidEntityChildId;
    }
    EntityId entId;
    entId.setRawId(entityId);
    return EDITOR_APP->createChildEntity(entId, childName);
}