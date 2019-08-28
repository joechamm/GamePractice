#ifndef __ENTITY_HPP__
#define __ENTITY_HPP__

#include "Entity/ETEntityInterfaces.hpp"

#include <string>
#include <memory>

class EntityLogic;

class Entity : public ETNode<ETEntity> {

    using EntityLogicPtrT = std::unique_ptr<EntityLogic>;

public:

    Entity(const char* entityName, EntityId entId);
    ~Entity();

    void addLogic(EntityLogicPtrT&& logic);

    EntityId getEntityId() const { return entityId; }

    // ETEntity
    const char* ET_getName() const override;
    void ET_setParent(EntityId entId) override;
    void ET_addChild(EntityId entId) override;
    void ET_removeChild(EntityId entId) override;
    EntityId ET_getParentId() const override;
    const Transform& ET_getTransform() const override;
    void ET_setTransform(const Transform& transform) override;
    const std::vector<EntityId>& ET_getChildren() const override;

private:

    Transform tm;
    std::vector<EntityLogicPtrT> logics;
    std::vector<EntityId> children;
    std::string name;
    EntityId parentId;
    EntityId entityId;
};

#endif /* __ENTITY_HPP__ */