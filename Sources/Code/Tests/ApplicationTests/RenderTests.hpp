#ifndef __RENDER_TESTS_HPP__
#define __RENDER_TESTS_HPP__

#include <gtest/gtest.h>

#include <memory>

class VoidTestApplication;
class RenderTextureFramebuffer;

class RenderTests : public ::testing::Test {
protected:

    static void SetUpTestCase();
    static void TearDownTestCase();

protected:

    static std::unique_ptr<VoidTestApplication> app;
    static std::unique_ptr<RenderTextureFramebuffer> textureFramebuffer;

protected:

    void SetUp() override;
    void TearDown() override;

protected:

    void checkSquare(size_t xStart, size_t xEnd, size_t yStart, size_t yEnd);
    void dumpFramebuffer();
};

#endif /* __RENDER_TESTS_HPP__ */