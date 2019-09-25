#ifndef __RENDER_COLORED_TEXTURE_LOGIC_HPP__
#define __RENDER_COLORED_TEXTURE_LOGIC_HPP__

#include "Render/Logics/RenderImageLogic.hpp"

class RenderColoredTextureLogic : public RenderImageLogic,
    public ETNode<ETRenderColoredTexture> {
public:

    RenderColoredTextureLogic();
    virtual ~RenderColoredTextureLogic();

    // EntityLogic
    bool serialize(const JSONNode& node) override;
    bool init() override;

    // ETRenderEvents
    void ET_onRender(const RenderContext& renderCtx) override;

    // ETRenderColoredTexture
    void ET_setTextureColor(const ColorB& newColor) override;

    // ETRenderImageLogic
    void ET_setImage(const char* imageName) override;

private:

    ColorB color;
};

#endif /* __RENDER_COLORED_TEXTURE_LOGIC_HPP__ */