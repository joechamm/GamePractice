#include "GLFWSurfaceTests.hpp"
#include "Platforms/Desktop/GLFWSurface.hpp"

TEST_F(GLFWSurfaceTest, TestInit) {
    GLFWSurface surface;
    bool res = surface.init();
    ASSERT_TRUE(res);

    int w = surface.getWidth();
    int h = surface.getHeight();

    ASSERT_GT(w, 0);
    ASSERT_GT(h, 0);
}

TEST_F(GLFWSurfaceTest, TestShouldRun) {
    GLFWSurface surface;
    ASSERT_TRUE(surface.init());
    bool res = surface.shouldRun();
    ASSERT_TRUE(res);
}

TEST_F(GLFWSurfaceTest, TestUpdate) {
    GLFWSurface surface;
    ASSERT_TRUE(surface.init());
    ASSERT_TRUE(surface.shouldRun());
    surface.update();
    surface.close();
    ASSERT_FALSE(surface.shouldRun());
}
