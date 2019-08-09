#include "UI/Logics/UIBaseBox.hpp"
#include "ETApplicationInterfaces.hpp"
#include "Render/ETRenderInterfaces.hpp"
#include "Game/ETGameInterfaces.hpp"
#include "Core/JSONNode.hpp"

#include <cassert>
#include <algorithm>

UIBaseBox::UIBaseBox() :
    style(),
    box(0),
    lastResizeBox(0) {
}

UIBaseBox::~UIBaseBox() {
}

void UIBaseBox::ET_onChildAdded(EntityId childId) {
    ET_SendEvent(childId, &ETUIBox::ET_boxResize);
}

void UIBaseBox::ET_alignInBox(const AABB2Di& alignBox) {
    Vec2i center = calcCenter(box, alignBox);
    box.setCenter(center);
    syncTransform();
}

void UIBaseBox::ET_setCenter(const Vec2i& center) {
    box.setCenter(center);
    syncTransform();
}

const AABB2Di& UIBaseBox::ET_getAabb2di() const {
    return box;
}

AABB2Di UIBaseBox::getParentAabb2di() const {
    AABB2Di parentAabb;
    parentAabb.bot = Vec2i(0);
    ET_SendEventReturn(parentAabb.top, &ETRenderCamera::ET_getRenderPort);
    auto parentId = getParentId();
    if(parentId != InvalidEntityId) {
        ET_SendEventReturn(parentAabb, parentId, &ETUIBox::ET_getAabb2di);
    }
    return parentAabb;
}

void UIBaseBox::ET_onTransformChanged(const Transform& newTm) {
    Vec2i newCenter = Vec2i(static_cast<int>(newTm.pt.x), static_cast<int>(newTm.pt.y));
    if(box.getCenter() != newCenter) {
        box.setCenter(newCenter);
    }
}

Vec2i UIBaseBox::calculateBoxSize(const AABB2Di& parentBox) const {
    Vec2i resSize(0);
    switch (style.sizeInv)
    {
        case SizeInvariant::Absolute:
        {
            Vec2i renderPort(0);
            ET_SendEventReturn(renderPort, &ETRenderCamera::ET_getRenderPort);
            resSize.x = static_cast<int>(renderPort.x * style.size.x);
            resSize.y = static_cast<int>(renderPort.y * style.size.y);
            break;
        }
        case SizeInvariant::AbsoluteBiggestSquare:
        {
            Vec2i renderPort(0);
            ET_SendEventReturn(renderPort, &ETRenderCamera::ET_getRenderPort);
            int minSide = std::min(renderPort.x, renderPort.y);
            resSize.x = static_cast<int>(minSide * style.size.x);
            resSize.y = static_cast<int>(minSide * style.size.y);
            break;
        }
        case SizeInvariant::Relative:
        {
            Vec2i parentSize = parentBox.getSize();
            resSize.x = static_cast<int>(parentSize.x * style.size.x);
            resSize.y = static_cast<int>(parentSize.y * style.size.y);
            break;
        }
        case SizeInvariant::RelativeBiggestSquare:
        {
            Vec2i parentSize = parentBox.getSize();
            int minSide = std::min(parentSize.x, parentSize.y);
            resSize.x = static_cast<int>(minSide * style.size.x);
            resSize.y = static_cast<int>(minSide * style.size.y);
            break;
        }
        case SizeInvariant::Pixel:
        {
            resSize.x = static_cast<int>(style.size.x);
            resSize.y = static_cast<int>(style.size.y);
            break;
        }
        default:
            assert(false && "Invalid style size invariant type");
            break;
    }
    return resSize;
}

Vec2i UIBaseBox::calcCenter(const AABB2Di& selfBox, const AABB2Di& parentBox) const {
    Vec2i resCenter = selfBox.getCenter();
    const auto halfSize = selfBox.getSize() / 2;
    switch (style.xAlignType) {
        case XAlignType::Center: {
            resCenter.x = parentBox.getCenter().x;
            break;
        }
        case XAlignType::Left: {
            resCenter.x = parentBox.bot.x + halfSize.x;
            break;
        }
        case XAlignType::Right: {
            resCenter.x = parentBox.top.x - halfSize.x;
            break;
        }
        default:
            assert(false && "Invalid align type");
    }
    switch (style.yAlignType) {
        case YAlignType::Center: {
            resCenter.y = parentBox.getCenter().y;
            break;
        }
        case YAlignType::Top: {
            resCenter.y = parentBox.top.y - halfSize.y;
            break;
        }
        case YAlignType::Bot: {
            resCenter.y = parentBox.bot.y + halfSize.y;
            break;
        }
        default:
            assert(false && "Invalid align type");
    }
    return resCenter;
}

void UIBaseBox::setBox(const AABB2Di& newBox) {
    box = newBox;
    syncTransform();
}

void UIBaseBox::syncTransform() const {
    Transform tm;
    auto center = box.getCenter();
    ET_SendEventReturn(tm, getEntityId(), &ETGameObject::ET_getTransform);
    tm.pt = Vec3(static_cast<float>(center.x), static_cast<float>(center.y), 0.f);
    ET_SendEvent(getEntityId(), &ETGameObject::ET_setTransform, tm);
}

bool UIBaseBox::isNeedResize() {
    auto currParentBox = getParentAabb2di();
    if (currParentBox == lastResizeBox) {
        return false;
    }
    lastResizeBox = currParentBox;
    return true;
}

void UIBaseBox::ET_boxResize() {
    if(!isNeedResize()) {
        return;
    }
    setBox(calcBox(getParentAabb2di()));
    std::vector<EntityId> childrenIds;
    ET_SendEventReturn(childrenIds, getEntityId(), &ETGameObject::ET_getChildren);
    for(auto childId : childrenIds) {
        ET_SendEvent(childId, &ETUIBox::ET_boxResize);
    }
    ET_SendEvent(getEntityId(), &ETUIBoxEvents::ET_onBoxResized);
}

void UIBaseBox::ET_onRenderPortResized() {
    if(!ET_IsExistNode<ETUIList>(getParentId())) {
        ET_boxResize();
    }
}

AABB2Di UIBaseBox::calcBox(const AABB2Di& parentBox) const {
    AABB2Di resAabb;
    resAabb.bot = Vec2i(0);
    resAabb.top = calculateBoxSize(parentBox);
    resAabb.setCenter(calcCenter(resAabb, parentBox));
    return resAabb;
}

bool UIBaseBox::serialize(const JSONNode& node) {
    auto styleNode = node.object("style");
    if(!styleNode) {
        LogWarning("[UIBaseBox::init] Can't find require style node");
        return false;
    }
    style.serialize(styleNode);
    return true;
}

const UIStyle& UIBaseBox::ET_getStyle() const {
    return style;
}

void UIBaseBox::ET_setStyle(const UIStyle& newStyle) {
    style = newStyle;
    forceResizeFromTop();
}

bool UIBaseBox::init() {
    setBox(calcBox(getParentAabb2di()));
    ETNode<ETUIBox>::connect(getEntityId());
    ETNode<ETRenderEvents>::connect(getEntityId());
    ETNode<ETGameObjectEvents>::connect(getEntityId());
    return true;
}

void UIBaseBox::forceResizeFromTop() {
    lastResizeBox = AABB2Di(0);
    auto rootEntId = getRootUIList();
    if(rootEntId == InvalidEntityId) {
        ET_boxResize();
    } else {
        ET_SendEvent(rootEntId, &ETUIBox::ET_boxResize);
    }
}

EntityId UIBaseBox::getRootUIList() const {
    auto entId = getParentId();
    auto lastValidEntId = entId;
    while(entId != InvalidEntityId) {
        lastValidEntId = entId;
        ET_SendEventReturn(entId, entId, &ETGameObject::ET_getParentId);
        if (!ET_IsExistNode<ETUIList>(entId)) {
            break;
        }
    }
    return lastValidEntId;
}