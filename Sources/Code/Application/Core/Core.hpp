#ifndef __CORE_HPP__
#define __CORE_HPP__

#include <functional>

class EntityId {

    static const int SYSTEM_MODULE_ID = 0;
    static const int INVALID_ID = 1;

public:

    static EntityId CreateSystemId() {
        EntityId id;
        id.setRawId(SYSTEM_MODULE_ID);
        return id;
    }

public:

    EntityId() : id(INVALID_ID) {}
    EntityId(const EntityId& entId) : id(entId.id) {}
    EntityId& operator=(const EntityId& endId) { id = endId.id; return *this; }
    ~EntityId() = default;
    bool operator==(const EntityId& entId) const { return id == entId.id; }
    bool operator!=(const EntityId& entId) const { return id != entId.id; }
    bool isValid() const { return id != INVALID_ID; }

    void setRawId(int entId) { id = entId; }
    int getRawId() const { return id; }
    void setInvalidId() { id = INVALID_ID; }

private:

    int id;
};

namespace std {
    template <>
    struct hash<EntityId> {
        size_t operator()(const EntityId& entId) const {
            return std::hash<int>()(entId.getRawId());
        }
    };
}

const EntityId InvalidEntityId;
const EntityId SystemEntityId = EntityId::CreateSystemId();

typedef int TypeId;
const TypeId InvalidTypeId = 0;

#endif /* __CORE_HPP__ */