#ifndef __RENDER_SIMPLE_LOGIC_HPP__
#define __RENDER_SIMPLE_LOGIC_HPP__

#include "Render/Logics/RenderNode.hpp"

#include <memory>

class RenderMaterial;
class RenderGeometry;

class RenderSimpleLogic : public RenderNode,
    public ETNode<ETRenderSimpleLogic>,
    public ETNode<ETRenderRect> {
public:

    RenderSimpleLogic();
    virtual ~RenderSimpleLogic();

    // EntityLogic
    bool serialize(const JSONNode& node) override;
    bool init() override;

    // ETRenderEvents
    void ET_onRender(const RenderContext& renderCtx) override;
    void ET_onRenderPortResized() override { }

    // ETRenderSimpleLogic
    void ET_setColor(const ColorB& col) override;

    // ETRenderRect
    void ET_setSize(const Vec2i& newSize) override;
    Vec2i ET_getSize() const override;

private:

    Vec2 scale;
    ColorB color;
};

#endif /* __RENDER_SIMPLE_LOGIC_HPP__ */