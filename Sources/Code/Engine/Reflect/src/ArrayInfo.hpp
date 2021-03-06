#ifndef __ARRAY_INFO_HPP__
#define __ARRAY_INFO_HPP__

#include "Reflect/ClassValue.hpp"

class ArrayInfo {
public:

    using CreateElemFuncT = ReflectUtils::ArrayCreateElemFuncT;
    using SizeFuncT = ReflectUtils::ArraySizeFuncT;
    using GetElemFuncT = ReflectUtils::ArrayGetElemFuncT;
    using ResetFuncT = ReflectUtils::ArrayResetFuncT;

public:

    ArrayInfo(TypeId elemTypeId, ClassValueType elemType, CreateElemFuncT createF, SizeFuncT sizeF,
        GetElemFuncT getElemF, ResetFuncT resetF);
    ~ArrayInfo();

    TypeId getElemTypeId() const;
    const char* getName() const;
    void makeReflectModel(JSONNode& node);
    bool writeValuesTo(void* valuePtr, MemoryStream& stream);
    bool readValuesFrom(void* valuePtr, const JSONNode& node);
    bool readValuesFrom(void* valuePtr, MemoryStream& stream);
    void setDefaultValue(void* valuePtr);
    bool addElement(void* valuePtr);

private:

    std::string name;
    ClassValue elemValue;
    CreateElemFuncT createFunc;
    SizeFuncT sizeFunc;
    GetElemFuncT getElemFunc;
    ResetFuncT resetFunc;
};

#endif /* __ARRAY_INFO_HPP__ */