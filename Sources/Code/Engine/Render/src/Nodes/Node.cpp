#include "Nodes/Node.hpp"
#include "RenderGraph/RenderGraph.hpp"
#include "Render/ETRenderManager.hpp"
#include "Core/ETPrimitives.hpp"
#include "Logics/RenderAuxFunctions.hpp"

#include <cassert>

Node::Node() :
    renderGraph(nullptr),
    alpha(1.f),
    drawPriority(0),
    blending(RenderBlendingType::NONE),
    visible(false) {
}

Node::~Node() {
}


void Node::initConnection(EntityId renderNodeId, const RenderNodeCreateParams& params) {
    nodeId = renderNodeId;
    alpha = params.alpha;
    drawPriority = params.drawPriority;
    tm = params.tm;
    ETNode<ETRenderProxyNode>::connect(nodeId);
}

void Node::initRender() {
    onInit();
}

void Node::ET_setAlpha(float newAlpha) {
    if(Render::IsRenderThread()) {
        alpha = newAlpha;
    } else {
        ET_QueueEvent(nodeId, &ETRenderProxyNode::ET_setAlpha, newAlpha);
    }
}

void Node::ET_setDrawPriority(int newDrawPriority) {
    if(Render::IsRenderThread()) {
        drawPriority = newDrawPriority;
        if(renderGraph) {
            renderGraph->setNeedReorderNodes();
        }
    } else {
        ET_QueueEvent(nodeId, &ETRenderProxyNode::ET_setDrawPriority, newDrawPriority);
    }
}

void Node::ET_setVisible(bool flag) {
    if(Render::IsRenderThread()) {
        visible = flag;
    } else {
        ET_QueueEvent(nodeId, &ETRenderProxyNode::ET_setVisible, flag);
    }
}

void Node::setBlendingMode(RenderBlendingType newBlending) {
    blending = newBlending;
}

void Node::setMaterial(const char* matName) {
    ET_SendEventReturn(mat, &ETRenderMaterialManager::ET_createMaterial, matName);
}

void Node::setGeometry(PrimitiveGeometryType geomType) {
    ET_SendEventReturn(geom, &ETRenderGeometryManager::ET_createGeometry, geomType);
}

void Node::ET_setTransform(const Transform& newTm) {
    assert(Render::IsRenderThread() && "Sync transfrom not in render thread");
    tm = newTm;
}

bool Node::isVisible() const {
    if(!mat) {
        return false;
    }
    if(!geom) {
        return false;
    }
    return visible;
}

void Node::setRenderGraph(RenderGraph* graph) {
    renderGraph = graph;
}

EntityId Node::getNodeId() const {
    return nodeId;
}

int Node::getDrawPriority() const {
    return drawPriority;
}

void Node::render() {
    if(!isVisible()) {
        return;
    }

    auto& renderCtx = renderGraph->getContext();
    if(blending != RenderBlendingType::NONE) {
        renderCtx.setBlending(blending);
    } else {
        if(alpha < 1.f) {
            renderCtx.setBlending(RenderBlendingType::ONE_MINUS_SRC_MINUS_ALPHA);
        } else {
            renderCtx.setBlending(blending);
        }
    }

    mat->bind();
    mat->setUniform1f("alpha", alpha);
    onRender(renderCtx);
    mat->unbind();
}