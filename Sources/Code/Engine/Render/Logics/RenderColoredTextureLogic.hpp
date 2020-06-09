#ifndef __RENDER_COLORED_TEXTURE_LOGIC_HPP__
#define __RENDER_COLORED_TEXTURE_LOGIC_HPP__

#include "Render/Logics/RenderImageLogic.hpp"

class RenderColoredTextureLogic : public RenderImageLogic,
    public ETNode<ETRenderColoredTexture> {
public:

    static void Reflect(ReflectContext& ctx);

public:

    RenderColoredTextureLogic();
    virtual ~RenderColoredTextureLogic();

    // EntityLogic
    bool init() override;

    // ETRenderColoredTexture
    void ET_setTextureColor(const ColorB& newColor) override;

protected:

    // RenderNode
    void onRender(RenderContext& renderCtx) override;

private:

    ColorB color;
};

#endif /* __RENDER_COLORED_TEXTURE_LOGIC_HPP__ */