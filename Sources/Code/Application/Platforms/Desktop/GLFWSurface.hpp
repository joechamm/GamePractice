#ifndef __GLFWS_SURFACE_HPP__
#define __GLFWS_SURFACE_HPP__

#include "ETApplicationInterfaces.hpp"
#include "Core/SystemLogic.hpp"

#include <memory>
#include <vector>

class GlfwLibInitData;
struct GLFWwindow;

class GLFWSurface : public SystemLogic,
    public ETNode<ETSurface>,
    public ETNode<ETTimerEvents> {
public:

    GLFWSurface();
    virtual ~GLFWSurface();

    // SystemLogic
    bool init() override;
    void deinit() override;

    // ETSurface
    bool ET_show() override;
    bool ET_hide() override;
    void ET_close() override;
    void ET_swapBuffers() override;
    Vec2i ET_getSize() const override;
    GLContextType ET_getGLContextType() const override;
    bool ET_isVisible() const override;
    bool ET_isValid() const override;

    // ETTimerEvenets
    void ET_onTick(float dt) override;

private:

    static void SetCursorePosCallback(GLFWwindow* window, double x, double y);
    static void SetFramebufferSizeCallback(GLFWwindow* window, int w, int h);
    static void SetMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

private:

    static std::unique_ptr<GlfwLibInitData> GLFW;

private:

    GLFWwindow* window;
    Vec2i size;
    std::vector<Vec2i> activeGesture;
};

#endif /* __GLFWS_SURFACE_HPP__ */
