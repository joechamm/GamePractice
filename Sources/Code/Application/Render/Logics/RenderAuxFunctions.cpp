#include "Render/Logics/RenderAuxFunctions.hpp"
#include "Render/RenderGeometry.hpp"
#include "Math/MatrixTransform.hpp"
#include "Entity/ETEntityInterfaces.hpp"
#include "Core/JSONNode.hpp"

namespace Render {

void ReadColor(const JSONNode& node, ColorB& color) {
    if(!node) {
        return;
    }
    int val = 0;
    node.read("r", val);
    color.r = static_cast<uint8_t>(Math::Clamp(val, 0, 255));
    val = 0;
    node.read("g", val);
    color.g = static_cast<uint8_t>(Math::Clamp(val, 0, 255));
    val = 0;
    node.read("b", val);
    color.b = static_cast<uint8_t>(Math::Clamp(val, 0, 255));
    val = 255;
    node.read("a", val);
    color.a = static_cast<uint8_t>(Math::Clamp(val, 0, 255));
}

Mat4 CalcModelMat(EntityId entityId, const Vec3& scale, const RenderGeometry& geom) {
    Transform tm;
    ET_SendEventReturn(tm, entityId, &ETEntity::ET_getTransform);

    Mat4 resMat(1.f);
    const Vec3 center = geom.aabb.getCenter();
    Math::Translate(resMat, center);
    Math::Rotate(resMat, tm.quat);
    Math::Translate(resMat, tm.pt);
    Vec3 resScale = Vec3(scale.x * tm.scale.x, scale.y * tm.scale.y, scale.z * tm.scale.z);
    Math::Scale(resMat, resScale);

    return resMat;
}

Vec2 CalcGeomScaleForSize(const Vec2i& size, const RenderGeometry& geom) {
    const auto& aabbSize = geom.aabb.getSize();
    Vec2 scale = Vec2(static_cast<float>(size.x), static_cast<float>(size.y))
        / Vec2(aabbSize.x, aabbSize.y);
    return scale;
}

Vec2i CaclScaledGeomSize(const Vec2& scale, const RenderGeometry& geom) {
    const auto& aabbSize = geom.aabb.getSize();
    Vec2i size = Vec2i(static_cast<int>(aabbSize.x * scale.x),
        static_cast<int>(aabbSize.y * scale.y));
    return size;
}

} // namespace Render