#ifndef __UI_BOX_HPP__
#define __UI_BOX_HPP__

#include "UI/Logics/UIBaseBox.hpp"

class UIBox : public UIBaseBox,
    public ETNode<ETUILabeledBox> {
public:

    static void Reflect(ReflectContext& ctx);

public:
    UIBox();
    virtual ~UIBox();

    // UIBaseBox
    bool init() override;
    void deinit() override;

    // ETUIBox
    void ET_setBoxStyle(const UIBoxStyle& newStyle) override;
    void ET_boxResize() override;

    // ETUILabeledBox
    EntityId ET_getLabelId() const override;
    void ET_setLabelText(const char* text) override;
    const char* ET_getLabelText() const override;

protected:

    EntityId getRenderId() const override;

private:

    void createRenderer();
    void updateRenderParams();
    void updateRenderSize();

private:

    EntityId renderId;
};

#endif /* __UI_BOX_HPP__ */