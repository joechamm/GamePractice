#include "Nodes/GradientNode.hpp"
#include "RenderTexture.hpp"
#include "Render/ETRenderManager.hpp"
#include "Logics/RenderAuxFunctions.hpp"

#include <cassert>

GradientNode::GradientNode() :
    size(100),
    startCol(255, 255, 255),
    endCol(0, 0, 0),
    isVertical(true) {
}

GradientNode::~GradientNode() {
}

void GradientNode::onInit() {
    setGeometry(PrimitiveGeometryType::Sqaure_Tex);
    setMaterial("tex_vert_color");
    ET_SendEventReturn(tex, &ETRenderTextureManger::ET_createEmptyTexture, Vec2i(2), ETextureType::RGBA);
    if(tex) {
        tex->setTexLerpType(TexLerpType::Nearest, TexLerpType::Nearest);
        tex->setWrapType(TexWrapType::ClamToEdge, TexWrapType::ClamToEdge);
        updateTexData();
    } else {
        assert(false && "Can't create 2x2 texture for gradient");
    }
}

void GradientNode::onRender(RenderContext& ctx) {
    auto scale = Render::CalcGeomScaleForSize(size, *geom);
    Mat4 mvp = Render::CalcModelMat(tm, Vec3(scale, 1.f), *geom);
    mvp = ctx.proj2dMat * mvp;

    mat->setUniformMat4("MVP", mvp);
    mat->setTexture2D("tex", tex->texId);
    geom->draw();
}

bool GradientNode::isVisible() const {
    if(!tex) {
        return false;
    }
    return Node::isVisible();
}

void GradientNode::ET_setVertical(bool flag) {
    if(Render::IsRenderThread()) {
        isVertical = flag;
        updateTexData();
    } else {
        ET_QueueEvent(nodeId, &ETRenderProxyNode::ET_setVertical, flag);
    }
}

void GradientNode::ET_setColor0(const ColorB& newColor) {
    if(Render::IsRenderThread()) {
        startCol = newColor;
        updateTexData();
    } else {
        ET_QueueEvent(nodeId, &ETRenderProxyNode::ET_setColor0, newColor);
    }
}

void GradientNode::ET_setColor1(const ColorB& newColor) {
    if(Render::IsRenderThread()) {
        endCol = newColor;
        updateTexData();
    } else {
        ET_QueueEvent(nodeId, &ETRenderProxyNode::ET_setColor1, newColor);
    }
}

void GradientNode::ET_setSize(const Vec2i& newSize) {
    if(Render::IsRenderThread()) {
        size = newSize;
    } else {
        ET_QueueEvent(nodeId, &ETRenderProxyNode::ET_setSize, newSize);
    }
}

void GradientNode::updateTexData() {
    if(!tex) {
        return;
    }
    if(isVertical) {
        ColorB vertColors[] = {
            startCol,
            startCol,
            endCol,
            endCol
        };
        tex->modifyRGBA(Vec2i(0), Vec2i(2), vertColors);
    } else {
        ColorB vertColors[] = {
            startCol,
            endCol,
            startCol,
            endCol
        };
        tex->modifyRGBA(Vec2i(0), Vec2i(2), vertColors);
    }
}