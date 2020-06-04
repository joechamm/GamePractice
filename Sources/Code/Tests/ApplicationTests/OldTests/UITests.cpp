#include "UITests.hpp"
#include "UI/Logics/UIBox.hpp"
#include "UI/Logics/UIList.hpp"
#include "Core/JSONNode.hpp"
#include "Render/ETRenderInterfaces.hpp"
#include "Entity/Entity.hpp"
#include "TestUtils/UITestUtils.hpp"

#include <algorithm>

UIList* UITests::createUIList() {
    auto object = createVoidObject();
    std::unique_ptr<UIList> uiListPtr(new UIList);
    UIList* uiList = uiListPtr.get();
    object->addCustomLogic(std::move(uiListPtr));
    tempObject.push_back(std::move(object));
    return uiList;
}

TEST_F(UITests, CheckUIBoxUpdatedAfterUpdatedTransform) {
    auto object = createUIBox();
    UIStyle style;
    style.size = Vec2(0.5f);
    object->ET_setStyle(style);
    ASSERT_TRUE(object->init());

    auto origCenter = object->ET_getAabb2di().getCenter();

    Vec2i renderPort(0);
    ET_SendEventReturn(renderPort, &ETRenderCamera::ET_getRenderPort);

    Transform tm;
    ET_SendEventReturn(tm, object->getEntityId(), &ETEntity::ET_getTransform);

    auto shift = renderPort.x / 2;
    tm.pt.x += static_cast<float>(shift);
    ET_SendEvent(object->getEntityId(), &ETEntity::ET_setTransform, tm);

    auto shfitCenter = object->ET_getAabb2di().getCenter();

    EXPECT_EQ(shfitCenter.x, origCenter.x + shift);
    EXPECT_EQ(shfitCenter.y, origCenter.y);
}

TEST_F(UITests, CheckUIBoxInsideUIBox) {
    UIBox* uiBox1 = createUIBox();
    UIBox* uiBox2 = createUIBox();

    ET_SendEvent(uiBox1->getEntityId(), &ETEntity::ET_addChild, uiBox2->getEntityId());

    UIStyle style;
    style.size = Vec2(0.5f, 0.5f);

    uiBox1->ET_setStyle(style);
    ASSERT_TRUE(uiBox1->init());

    uiBox2->ET_setStyle(style);
    ASSERT_TRUE(uiBox2->init());

    auto& parentBox = uiBox1->ET_getAabb2di();
    auto parentSize = parentBox.getSize();
    auto& aabb = uiBox2->ET_getAabb2di();

    ASSERT_EQ(aabb.getSize(), Vec2i(parentSize / 2));
    ASSERT_EQ(aabb.getCenter(), parentBox.getCenter());

    Transform box1Tm;
    ET_SendEventReturn(box1Tm, uiBox1->getEntityId(), &ETEntity::ET_getTransform);

    Transform box2Tm;
    ET_SendEventReturn(box2Tm, uiBox2->getEntityId(), &ETEntity::ET_getTransform);

    EXPECT_FLOAT_EQ(box1Tm.pt.x, box2Tm.pt.x);
    EXPECT_FLOAT_EQ(box1Tm.pt.y, box2Tm.pt.y);
}

TEST_F(UITests, CheckHorizontalUIList) {
    UIList* uiList = createUIList();
    uiList->ET_setType(UIListType::Horizontal);
    ASSERT_TRUE(uiList->init());

    UIStyle boxStyle;
    boxStyle.size = Vec2(0.25f, 0.25f);

    UIBox* uiBox1 = createUIBox();
    uiBox1->ET_setStyle(boxStyle);
    ASSERT_TRUE(uiBox1->init());

    UIBox* uiBox2 = createUIBox();
    uiBox2->ET_setStyle(boxStyle);
    ASSERT_TRUE(uiBox2->init());

    ET_SendEvent(uiList->getEntityId(), &ETEntity::ET_addChild, uiBox1->getEntityId());
    ET_SendEvent(uiList->getEntityId(), &ETEntity::ET_addChild, uiBox2->getEntityId());

    const auto& aabbList = uiList->ET_getAabb2di();

    Vec2i renderPort(0);
    ET_SendEventReturn(renderPort, &ETRenderCamera::ET_getRenderPort);

    Vec2i expCenter = renderPort / 2;
    Vec2i resCenter = aabbList.getCenter();
    ASSERT_EQ(resCenter, expCenter);
    Vec2i expSize = Vec2i(renderPort.x / 2, renderPort.y / 4);
    Vec2i resSize = aabbList.getSize();
    ASSERT_EQ(resSize, expSize);

    const auto& aabbBox1 = uiBox1->ET_getAabb2di();
    expCenter = Vec2i(3 * renderPort.x / 8, renderPort.y / 2);
    resCenter = aabbBox1.getCenter();
    ASSERT_EQ(resCenter, expCenter);
    expSize = renderPort / 4;
    resSize = aabbBox1.getSize();
    ASSERT_EQ(resSize, expSize);

    const auto& aabbBox2 = uiBox2->ET_getAabb2di();
    expCenter = Vec2i(5 * renderPort.x / 8, renderPort.y / 2);
    resCenter = aabbBox2.getCenter();
    ASSERT_EQ(resCenter, expCenter);
    expSize = renderPort / 4;
    resSize = aabbBox2.getSize();
    ASSERT_EQ(resSize, expSize);
}

TEST_F(UITests, CheckUIListResize) {
    auto rootUiBox = createUIBox();
    UIStyle rootBoxStyle;
    rootBoxStyle.size = Vec2(0.5f);
    rootUiBox->ET_setStyle(rootBoxStyle);
    ASSERT_TRUE(rootUiBox->init());

    auto uiList = createUIList();
    UIStyle uiListStyle;
    uiList->ET_setStyle(uiListStyle);
    ASSERT_TRUE(uiList->init());

    auto innerUiBox = createUIBox();
    UIStyle innerBoxStyle;
    innerBoxStyle.size = Vec2(0.5f);
    innerUiBox->ET_setStyle(innerBoxStyle);
    ASSERT_TRUE(innerUiBox->init());

    ET_SendEvent(rootUiBox->getEntityId(), &ETEntity::ET_addChild, uiList->getEntityId());
    ET_SendEvent(uiList->getEntityId(), &ETEntity::ET_addChild, innerUiBox->getEntityId());

    Vec2i renderPort(0);
    ET_SendEventReturn(renderPort, &ETRenderCamera::ET_getRenderPort);

    auto aabb = innerUiBox->ET_getAabb2di();

    auto size = aabb.getSize();
    auto center = aabb.getCenter();
    EXPECT_EQ(size, renderPort / 4);
    EXPECT_EQ(center, renderPort / 2);

    rootBoxStyle.size = Vec2(1.f);
    rootUiBox->ET_setStyle(rootBoxStyle);

    aabb = innerUiBox->ET_getAabb2di();

    size = aabb.getSize();
    center = aabb.getCenter();
    EXPECT_EQ(size, renderPort / 2);
    EXPECT_EQ(center, renderPort / 2);
}

TEST_F(UITests, CheckUIListCombined) {
    auto rootVertUIList = createUIList();
    {
        UIStyle rootListStyle;
        rootListStyle.size = Vec2(1.f);
        rootListStyle.sizeInv = SizeInvariant::Relative;
        rootVertUIList->ET_setType(UIListType::Vertical);
        ASSERT_TRUE(rootVertUIList->init());
    }

    auto topUIBox = createUIBox();
    {
        UIStyle topBoxStyle;
        topBoxStyle.size = Vec2(0.5f);
        topBoxStyle.sizeInv = SizeInvariant::Relative;
        topUIBox->ET_setStyle(topBoxStyle);
        ASSERT_TRUE(topUIBox->init());
    }

    ET_SendEvent(rootVertUIList->getEntityId(), &ETEntity::ET_addChild, topUIBox->getEntityId());

    auto leftUIBox = createUIBox();
    {
        UIStyle leftBoxStyle;
        leftBoxStyle.size = Vec2(0.5f);
        leftBoxStyle.sizeInv = SizeInvariant::Relative;
        leftUIBox->ET_setStyle(leftBoxStyle);
        ASSERT_TRUE(leftUIBox->init());
    }

    auto rightUIBox = createUIBox();
    {
        UIStyle rightBoxStyle;
        rightBoxStyle.size = Vec2(0.5f);
        rightBoxStyle.sizeInv = SizeInvariant::Relative;
        rightUIBox->ET_setStyle(rightBoxStyle);
        ASSERT_TRUE(rightUIBox->init());
    }

    auto botHorzUIlist = createUIList();
    {
        UIStyle botListStyle;
        botListStyle.size = Vec2(0.5f);
        botListStyle.sizeInv = SizeInvariant::Relative;
        botHorzUIlist->ET_setType(UIListType::Horizontal);
        ASSERT_TRUE(botHorzUIlist->init());
    }

    ET_SendEvent(botHorzUIlist->getEntityId(), &ETEntity::ET_addChild, leftUIBox->getEntityId());
    ET_SendEvent(botHorzUIlist->getEntityId(), &ETEntity::ET_addChild, rightUIBox->getEntityId());

    ET_SendEvent(rootVertUIList->getEntityId(), &ETEntity::ET_addChild, botHorzUIlist->getEntityId());

    Vec2i renderPort(0);
    ET_SendEventReturn(renderPort, &ETRenderCamera::ET_getRenderPort);

    auto rootBox = rootVertUIList->ET_getAabb2di();
    auto rootBoxSize = rootBox.getSize();
    EXPECT_EQ(rootBoxSize, renderPort);
    EXPECT_EQ(rootBox.getCenter(), renderPort / 2);

    auto botBox = botHorzUIlist->ET_getAabb2di();
    auto botBoxSize = botBox.getSize();
    EXPECT_EQ(botBoxSize, Vec2i(renderPort.x, renderPort.y / 2));
    EXPECT_EQ(botBox.getCenter(), Vec2i(renderPort.x / 2, renderPort.y / 4));

    auto leftBox = leftUIBox->ET_getAabb2di();
    auto leftBoxSize = leftBox.getSize();
    EXPECT_EQ(leftBoxSize, renderPort / 2);

    auto rightBox = rightUIBox->ET_getAabb2di();
    auto rightBoxSize = rightBox.getSize();
    EXPECT_EQ(rightBoxSize, renderPort / 2);
}

TEST_F(UITests, CheckListWithAlignVariation) {
    UIList* uiList = createUIList();
    uiList->ET_setType(UIListType::Vertical);
    ASSERT_TRUE(uiList->init());

    UIStyle boxStyle;
    boxStyle.size = Vec2(0.5);
    boxStyle.xAlignType = XAlignType::Left;

    UIBox* leftUIBox = createUIBox();
    leftUIBox->ET_setStyle(boxStyle);
    ASSERT_TRUE(leftUIBox->init());

    boxStyle.xAlignType = XAlignType::Right;

    UIBox* rightUIBox = createUIBox();
    rightUIBox->ET_setStyle(boxStyle);
    ASSERT_TRUE(rightUIBox->init());

    ET_SendEvent(uiList->getEntityId(), &ETEntity::ET_addChild, leftUIBox->getEntityId());
    ET_SendEvent(uiList->getEntityId(), &ETEntity::ET_addChild, rightUIBox->getEntityId());

    Vec2i renderPort(0);
    ET_SendEventReturn(renderPort, &ETRenderCamera::ET_getRenderPort);

    auto listBox = uiList->ET_getAabb2di();
    EXPECT_EQ(listBox.getSize(), renderPort);
    EXPECT_EQ(listBox.getCenter(), renderPort / 2);

    auto leftBox = leftUIBox->ET_getAabb2di();
    EXPECT_EQ(leftBox.getSize(), renderPort / 2);
    EXPECT_EQ(leftBox.getCenter(), Vec2i(renderPort.x / 4, 3 * renderPort.y /4));

    auto rightBox = rightUIBox->ET_getAabb2di();
    EXPECT_EQ(rightBox.getSize(), renderPort / 2);
    EXPECT_EQ(rightBox.getCenter(), Vec2i(3 * renderPort.x / 4, renderPort.y /4));
}

TEST_F(UITests, CheckUIElementResized) {
    auto uiList = createUIList();
    {
        UIStyle style;
        style.size = Vec2(400.f);
        style.sizeInv = SizeInvariant::Pixel;
        uiList->ET_setType(UIListType::Horizontal);
        uiList->ET_setStyle(style);
        ASSERT_TRUE(uiList->init());
    }
    auto firstBox = createUIBox();
    {
        UIStyle style;
        style.size = Vec2(0.25f);
        style.sizeInv = SizeInvariant::Relative;
        firstBox->ET_setStyle(style);
        ASSERT_TRUE(firstBox->init());
    }
    auto secondBox = createUIBox();
    {
        UIStyle style;
        style.size = Vec2(0.25f);
        style.sizeInv = SizeInvariant::Relative;
        secondBox->ET_setStyle(style);
        ASSERT_TRUE(secondBox->init());
    }
    
    ET_SendEvent(uiList->getEntityId(), &ETEntity::ET_addChild, firstBox->getEntityId());
    ET_SendEvent(uiList->getEntityId(), &ETEntity::ET_addChild, secondBox->getEntityId());

    auto listBox = uiList->ET_getAabb2di();
    auto listBoxSize = listBox.getSize();

    EXPECT_EQ(listBoxSize.x, 200);
    EXPECT_EQ(listBoxSize.y, 100);

    {
        UIStyle style = secondBox->ET_getStyle();
        style.size = Vec2(0.75f, 0.25f);
        secondBox->ET_setStyle(style);
    }

    listBox = uiList->ET_getAabb2di();
    listBoxSize = listBox.getSize();

    EXPECT_EQ(listBoxSize.x, 400);
    EXPECT_EQ(listBoxSize.y, 100);
}

TEST_F(UITests, CheckUIListTransformInsideBox) {
    Vec2i uiBoxPt = Vec2i(200);
    auto uiBox = createUIBox();
    {
        UIStyle style;
        style.size = Vec2(1.f);
        uiBox->ET_setStyle(style);
        ASSERT_TRUE(uiBox->init());
        Transform tm;
        tm.pt = Vec3(static_cast<float>(uiBoxPt.x), static_cast<float>(uiBoxPt.y), 0.f);
        ET_SendEvent(uiBox->getEntityId(), &ETEntity::ET_setTransform, tm);
    }

    auto uiList = createUIList();
    {
        UIStyle style;
        style.size = Vec2(1.f);
        uiList->ET_setType(UIListType::Vertical);
        ASSERT_TRUE(uiList->init());
    }

    auto childBox = createUIBox();
    {
        UIStyle style;
        style.size = Vec2(0.5f);
        childBox->ET_setStyle(style);
        ASSERT_TRUE(childBox->init());
    }

    ET_SendEvent(uiBox->getEntityId(), &ETEntity::ET_addChild, uiList->getEntityId());
    ET_SendEvent(uiList->getEntityId(), &ETEntity::ET_addChild, childBox->getEntityId());

    uiBoxPt = Vec2i(400, 200);

    {
        Transform tm;
        tm.pt = Vec3(static_cast<float>(uiBoxPt.x), static_cast<float>(uiBoxPt.y), 0.f);
        ET_SendEvent(uiBox->getEntityId(), &ETEntity::ET_setTransform, tm);
    }

    {
        Transform tm;
        ET_SendEventReturn(tm, uiList->getEntityId(), &ETEntity::ET_getTransform);
        EXPECT_EQ(static_cast<int>(tm.pt.x), uiBoxPt.x);
        EXPECT_EQ(static_cast<int>(tm.pt.y), uiBoxPt.y);
    }

    {
        Transform tm;
        ET_SendEventReturn(tm, childBox->getEntityId(), &ETEntity::ET_getTransform);
        EXPECT_EQ(static_cast<int>(tm.pt.x), uiBoxPt.x);
        EXPECT_EQ(static_cast<int>(tm.pt.y), uiBoxPt.y);
    }
}

TEST_F(UITests, CheckUIListAlignLeftBot) {
    auto uiBox = createObjectAndLogic<UIBaseBox>();
    {
        UIStyle style;
        style.size = Vec2(0.5f);
        uiBox->ET_setStyle(style);
        ASSERT_TRUE(uiBox->init());
    }
    auto uiList = createObjectAndLogic<UIList>();
    {
        UIStyle style;
        style.xAlignType = XAlignType::Left;
        style.yAlignType = YAlignType::Bot;
        uiList->ET_setStyle(style);
        uiList->ET_setType(UIListType::Horizontal);
        ASSERT_TRUE(uiList->init());
    }
    ET_SendEvent(uiList->getEntityId(), &ETEntity::ET_addChild, uiBox->getEntityId());

    Vec2i renderPort(0);
    ET_SendEventReturn(renderPort, &ETRenderCamera::ET_getRenderPort);

    Vec2i expectedSize = Vec2i(renderPort.x / 2, renderPort.y / 2);
    Vec2i expectedCenter = Vec2i(renderPort.x / 4, renderPort.y / 4);
    CheckUIBoxSizeAndCenter(uiBox, expectedSize, expectedCenter);
}

TEST_F(UITests, CheckUIListAlignLeftTop) {
    auto uiBox = createObjectAndLogic<UIBaseBox>();
    {
        UIStyle style;
        style.size = Vec2(0.5f);
        uiBox->ET_setStyle(style);
        ASSERT_TRUE(uiBox->init());
    }
    auto uiList = createObjectAndLogic<UIList>();
    {
        UIStyle style;
        style.xAlignType = XAlignType::Left;
        style.yAlignType = YAlignType::Top;
        uiList->ET_setStyle(style);
        uiList->ET_setType(UIListType::Horizontal);
        ASSERT_TRUE(uiList->init());
    }
    ET_SendEvent(uiList->getEntityId(), &ETEntity::ET_addChild, uiBox->getEntityId());

    Vec2i renderPort(0);
    ET_SendEventReturn(renderPort, &ETRenderCamera::ET_getRenderPort);

    Vec2i expectedSize = Vec2i(renderPort.x / 2, renderPort.y / 2);
    Vec2i expectedCenter = Vec2i(renderPort.x / 4, renderPort.y * 3 / 4);
    CheckUIBoxSizeAndCenter(uiBox, expectedSize, expectedCenter);
}

TEST_F(UITests, CheckUIListAlignRightTop) {
    auto uiBox = createObjectAndLogic<UIBaseBox>();
    {
        UIStyle style;
        style.size = Vec2(0.5f);
        uiBox->ET_setStyle(style);
        ASSERT_TRUE(uiBox->init());
    }
    auto uiList = createObjectAndLogic<UIList>();
    {
        UIStyle style;
        style.xAlignType = XAlignType::Right;
        style.yAlignType = YAlignType::Top;
        uiList->ET_setStyle(style);
        uiList->ET_setType(UIListType::Horizontal);
        ASSERT_TRUE(uiList->init());
    }
    ET_SendEvent(uiList->getEntityId(), &ETEntity::ET_addChild, uiBox->getEntityId());

    Vec2i renderPort(0);
    ET_SendEventReturn(renderPort, &ETRenderCamera::ET_getRenderPort);

    Vec2i expectedSize = Vec2i(renderPort.x / 2, renderPort.y / 2);
    Vec2i expectedCenter = Vec2i(renderPort.x * 3 / 4, renderPort.y * 3 / 4);
    CheckUIBoxSizeAndCenter(uiBox, expectedSize, expectedCenter);
}

TEST_F(UITests, CheckUIListAlignRightBot) {
    auto uiBox = createObjectAndLogic<UIBaseBox>();
    {
        UIStyle style;
        style.size = Vec2(0.5f);
        uiBox->ET_setStyle(style);
        ASSERT_TRUE(uiBox->init());
    }
    auto uiList = createObjectAndLogic<UIList>();
    {
        UIStyle style;
        style.xAlignType = XAlignType::Right;
        style.yAlignType = YAlignType::Bot;
        uiList->ET_setStyle(style);
        uiList->ET_setType(UIListType::Horizontal);
        ASSERT_TRUE(uiList->init());
    }
    ET_SendEvent(uiList->getEntityId(), &ETEntity::ET_addChild, uiBox->getEntityId());

    Vec2i renderPort(0);
    ET_SendEventReturn(renderPort, &ETRenderCamera::ET_getRenderPort);

    Vec2i expectedSize = Vec2i(renderPort.x / 2, renderPort.y / 2);
    Vec2i expectedCenter = Vec2i(renderPort.x * 3 / 4, renderPort.y / 4);
    CheckUIBoxSizeAndCenter(uiBox, expectedSize, expectedCenter);
}