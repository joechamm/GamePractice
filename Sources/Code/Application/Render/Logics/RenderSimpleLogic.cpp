#include "Render/Logics/RenderSimpleLogic.hpp"
#include "Render/RenderGeometry.hpp"
#include "Render/RenderMaterial.hpp"
#include "Core/JSONNode.hpp"
#include "Math/MatrixTransform.hpp"
#include "Entity/ETEntityInterfaces.hpp"
#include "Render/RenderContext.hpp"
#include "Render/Logics/RenderAuxFunctions.hpp"

#include <cassert>

RenderSimpleLogic::RenderSimpleLogic() :
    scale(1.f),
    color(255, 255, 255) {
}

RenderSimpleLogic::~RenderSimpleLogic() {
}

bool RenderSimpleLogic::serialize(const JSONNode& node) {
    if(!RenderNode::serialize(node)) {
        return false;
    }
    return true;
}

bool RenderSimpleLogic::init() {
    if(!RenderNode::init()) {
        return false;
    }
    ETNode<ETRenderSimpleLogic>::connect(getEntityId());
    ETNode<ETRenderRect>::connect(getEntityId());
    return true;
}

void RenderSimpleLogic::ET_onRender(const RenderContext& renderCtx) {
    Mat4 mvp = Render::CalcModelMat(getEntityId(), Vec3(scale, 1.f), *geom);
    mvp = renderCtx.proj2dMat * mvp;

    mat->bind();
    mat->setUniformMat4("MVP", mvp);
    mat->setUniform4f("color", color);
    geom->draw();
    mat->unbind();
}

void RenderSimpleLogic::ET_setColor(const ColorB& col) {
    color = col;
}

void RenderSimpleLogic::ET_setSize(const Vec2i& newSize) {
    scale = Render::CalcGeomScaleForSize(newSize, *geom);
}

Vec2i RenderSimpleLogic::ET_getSize() const {
    return Render::CaclScaledGeomSize(scale, *geom);
}