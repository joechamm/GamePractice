#ifndef __UI_SURFACE_EVENT_HANDLER_HPP__
#define __UI_SURFACE_EVENT_HANDLER_HPP__

#include "Core/SystemLogic.hpp"
#include "Core/ETPrimitives.hpp"
#include "Platform/ETSurface.hpp"

class UISurfaceEventHandler : public SystemLogic,
    public ETNode<ETInputEvents>,
    public ETNode<ETSurfaceEvents> {
public:

    UISurfaceEventHandler();
    virtual ~UISurfaceEventHandler();

    // SystemLogic
    bool init() override;
    virtual void deinit() override;

    // ETInputEvents
    void ET_onTouch(EActionType actionType, const Vec2i& pt) override;
    void ET_onButton(EActionType actionType, EButtonId buttonId) override;

    // ETSurfaceEvents
    void ET_onSurfaceCreated() override {}
    void ET_onSurfaceDestroyed() override {}
    void ET_onSurfaceHidden() override;
    void ET_onSurfaceShown() override;
    void ET_onSurfaceLostFocus() override {}
    void ET_onSurfaceGainFocus() override {}
    void ET_onSurfaceResized(const Vec2i& size) override { (void)size; }

private:

    void onPress(const Vec2i& pt);
    void onMove(const Vec2i& pt);
    void onRelease(const Vec2i& pt);

    bool isHover(const Vec2i& pt, EntityId entId) const;
    EntityId getHoveredEntity(const Vec2i& pt) const;

private:

    EntityId pressElemId;
    EntityId hoveredElemId;
};

#endif /* __UI_SURFACE_EVENT_HANDLER_HPP__ */