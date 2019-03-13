#ifndef __UI_STYLE_HPP__
#define __UI_STYLE_HPP__

#include "Math/Vector.hpp"

class JSONNode;

enum class ListType {
    Vertical = 0,
    Horizontal
};

enum class AlignType {
    Center = 0,
    Left,
    Right
};

struct UIStyle {
public:

    ListType listType;
    AlignType alignType;
    Vec2 size;

public:

    UIStyle();
    ~UIStyle();
    void serialize(const JSONNode& node);
};

#endif /* __UI_STYLE_HPP__ */