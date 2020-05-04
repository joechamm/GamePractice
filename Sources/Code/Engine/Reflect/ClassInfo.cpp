#include "Reflect/ClassInfo.hpp"
#include "Reflect/ETReflectInterfaces.hpp"
#include "Reflect/ReflectContext.hpp"
#include "Core/JSONNode.hpp"
#include "Core/ETPrimitives.hpp"
#include "ETApplicationInterfaces.hpp"

ClassInfo::ClassInfo(const char* name, TypeId typeId) :
    className(name),
    instanceTypeId(typeId) {
}

ClassInfo::~ClassInfo() {
}

ClassInstanceDeleteFuncT ClassInfo::getDeleteFunction() const {
    return deleteFunc;
}

TypeId ClassInfo::getIntanceTypeId() const {
    return instanceTypeId;
}

bool ClassInfo::serializeInstance(void* instance, const JSONNode& node) {
    if(!instance) {
        LogError("[ClassInfo::serializeInstance] Can't serialize instance of class '%s' (Error: null instance)",
            className);
        return false;
    }
    if(!node) {
        LogError("[ClassInfo::serializeInstance] Can't serialize instance of class '%s' (Error: invalid json node)",
            className);
        return false;
    }
    for(auto baseClass : baseClasses) {
        if(!baseClass->serializeInstance(instance, node)) {
            LogError("[ClassInfo::serializeInstance] Can't serialize instance of class '%s' (Error: can't serialize base class '%s')",
                className, baseClass->className);
            return false;
        }
    }
    for(auto& val : values) {
        auto ptr = getValueFunc(instance, val.ptr);
        if(!val.serializeValue(instance, ptr, node)) {
            LogError("[ClassInfo::serializeInstance] Can't serialize instance of class '%s' (Error: can't serialize value: '%s')",
                className, val.name);
            return false;
        }
    }
    return true;
}

const char* ClassInfo::getName() const {
    return className.c_str();
}

ClassInfo* ClassInfo::findClassInfo(TypeId instanceTypeId) const {
    ClassInfo* classInfo = nullptr;
    ET_SendEventReturn(classInfo, &ETClassInfoManager::ET_findClassInfoByTypeId, instanceTypeId);
    return classInfo;
}

void ClassInfo::registerClassValue(const char* valueName, ClassValueType valueType, ClassValue::ValuePtrT valuePtr, TypeId valueTypeId,
    ClassValue::SetResourceFuncT valueSetFunc) {
    const char* errStr = "[ClassInfo::registerClassValue] Can't register field '%s' for class %s (Error: %s)";
    if(!valueName || !valueName[0]) {
        LogError(errStr, className, valueName, "empty name");
        return;
    }
    if(valueType == ClassValueType::Invalid) {
        LogError(errStr, className, valueName, "unknown field type");
        return;
    }
    if(valueType == ClassValueType::Resource) {
        if(!valueSetFunc) {
            LogError(errStr, className, valueName, "resource without create function");
            return;
        }
    } else {
        if(!valuePtr) {
            LogError(errStr, className, valueName, "null pointer to value");
            return;
        }
        if(valueTypeId == InvalidTypeId) {
            LogError(errStr, className, valueName, "invalid type of field");
            return;
        }
    }
    std::vector<ClassInfo*> allBaseClasses;
    getAllClasses(allBaseClasses);
    for(auto baseClass : allBaseClasses) {
        if(baseClass->findValueByName(valueName)) {
            LogError(errStr, className, valueName,
                StringFormat("base class '%s' already registered field with the same name", baseClass->getName()));
            return;
        }
        if(auto baseValue = baseClass->findValueByPtr(valuePtr)) {
            LogError(errStr, className, valueName,
                StringFormat("base class '%s' already registered field with other name '%s'",
                baseClass->getName(), baseValue->name.c_str()));
            return;
        }
    }
    ClassValue classValue;
    classValue.name = valueName;
    classValue.type = valueType;
    classValue.ptr = valuePtr;
    classValue.typeId = valueTypeId;
    classValue.setResourceFunc = valueSetFunc;
    values.push_back(classValue);
}

ClassInstance ClassInfo::createInstance(const JSONNode& node) {
    if(!createFunc) {
        return ClassInstance();
    }
    auto object = createFunc();
    if(!object) {
        return ClassInstance();
    }
    if(!serializeInstance(object, node)) {
        deleteFunc(object);
        return ClassInstance();
    }
    return ClassInstance(*this, object);
}

void ClassInfo::getAllClasses(std::vector<ClassInfo*> classes) {
    classes.push_back(this);
    for(auto classInfo : baseClasses) {
        classes.push_back(classInfo);
        classInfo->getAllClasses(classes);
    }
}

void ClassInfo::registerBaseClass(ClassInfo* baseClassInfo) {
    const char* errStr = "[ClassInfo::registerBaseClass] Can't register the base class '%s' for the class '%s' (Error: %s)";
    if(!baseClassInfo) {
        LogError(errStr, "null", className, "invalid base class");
        return;
    }

    std::vector<ClassInfo*> newClasses;
    baseClassInfo->getAllClasses(newClasses);

    std::vector<ClassInfo*> currentClasses;
    getAllClasses(currentClasses);

    for(auto newClass : newClasses) {
        for(auto currentClass : currentClasses) {
            if(newClass == currentClass) {
                LogError(errStr, baseClassInfo->className, className, "class already registered as a base");
                return;
            }
            if(newClass->className == currentClass->className) {
                LogError(errStr, baseClassInfo->className, className, "class with the same name already registered");
                return;
            }
            if(newClass->instanceTypeId == currentClass->instanceTypeId) {
                LogError(errStr, baseClassInfo->className, className, "change name of the class");
                return;
            }
            for(auto& val : newClass->values) {
                if(currentClass->findValueByName(val.name.c_str())) {
                    LogError(errStr, baseClassInfo->className, className,
                        StringFormat("field '%s' already registered", val.name));
                    return;
                }
            }
        }
    }
    baseClasses.push_back(baseClassInfo);
}

ClassValue* ClassInfo::findValueByPtr(ClassValue::ValuePtrT ptr) {
    for(auto& val : values) {
        if(val.ptr == ptr) {
            return &val;
        }
    }
    return nullptr;
}

ClassValue* ClassInfo::findValueByName(const char* name) {
    if(!name || !name[0]) {
        return nullptr;
    }
    for(auto& val : values) {
        if(val.name == name) {
            return &val;
        }
    }
    return nullptr;
}

bool ClassInfo::reflectEmbebedClass(ReflectFuncT reflectFunc) {
    ReflectContext ctx;
    return ctx.reflectEmbedded(reflectFunc);
}

 void ClassInfo::makeReflectModel(JSONNode& node) {
    for(auto& value : values) {
        node.write(value.name.c_str(), value.getTypeName());
    }
}