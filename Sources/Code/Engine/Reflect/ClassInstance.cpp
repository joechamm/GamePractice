#include "Reflect/ClassInstance.hpp"
#include "Reflect/ClassInfo.hpp"
#include "ETApplicationInterfaces.hpp"

#include <cassert>

ClassInstance::ClassInstance() :
    classInfo(nullptr),
    instance(nullptr),
    deleteFunc(nullptr) {
}

ClassInstance::ClassInstance(ClassInstance&& other) :
    classInfo(other.classInfo),
    instance(other.instance),
    deleteFunc(other.deleteFunc) {

    other.instance = nullptr;
    other.classInfo = nullptr;
    other.deleteFunc = nullptr;
}

ClassInstance& ClassInstance::operator=(ClassInstance&& other) {
    if(this == &other) {
        return *this;
    }
    if(instance) {
        deleteFunc(instance);
    }
    instance = other.instance;
    classInfo = other.classInfo;
    deleteFunc = other.deleteFunc;
    other.instance = nullptr;
    other.classInfo = nullptr;
    other.deleteFunc = nullptr;
    return *this;
}

ClassInstance::ClassInstance(ClassInfo& clsInfo, void* clsInstance) :
    classInfo(&clsInfo),
    instance(clsInstance),
    deleteFunc(clsInfo.getDeleteFunction()) {
}

ClassInstance::~ClassInstance() {
    if(instance) {
        deleteFunc(instance);
    }
    instance = nullptr;
}

bool ClassInstance::isInstanceOfType(TypeId typeId) const {
    if(!classInfo) {
        return false;
    }
    return classInfo->getIntanceTypeId() == typeId;
}

TypeId ClassInstance::getInstanceTypeId() const {
    if(!classInfo) {
        return InvalidTypeId;
    }
    return classInfo->getIntanceTypeId();
}

void* ClassInstance::get() {
    return instance;
}

void ClassInstance::setDeleteFuncAndPtr(DeleteFuncT deleteF, void* ptr) {
    instance = ptr;
    deleteFunc = deleteF;
}

bool ClassInstance::readValues(MemoryStream& stream) {
    if(!instance) {
        assert(false && "Invalid instance");
        return false;
    }
    if(!classInfo) {
        LogError("[ClassInstance::readValues] Can't read values of instance without class info");
        return false;
    }
    return classInfo->readValues(instance, stream);
}

bool ClassInstance::readValue(EntityLogicValueId valueId, MemoryStream& stream) {
    if(!instance) {
        assert(false && "Invalid instance");
        return false;
    }
    if(!classInfo) {
        LogError("[ClassInstance::readValue] Can't read value of instance without class info");
        return false;
    }
    return classInfo->readValue(instance, valueId, stream);
}

bool ClassInstance::writeValues(MemoryStream& stream) {
    if(!instance) {
        assert(false && "Invalid instance");
        return false;
    }
    if(!classInfo) {
        LogError("[ClassInstance::writeValues] Can't write value of instance without class info");
        return false;
    }
    return classInfo->writeValues(instance, stream);
}

bool ClassInstance::writeValue(EntityLogicValueId valueId, MemoryStream& stream) {
    if(!instance) {
        assert(false && "Invalid instance");
        return false;
    }
    if(!classInfo) {
        LogError("[ClassInstance::writeValue] Can't write value of instance without class info");
        return false;
    }
    return classInfo->writeValue(instance, valueId, stream);
}