#include "UITests.hpp"
#include "UI/Logics/UIBox.hpp"
#include "UI/Logics/UIList.hpp"
#include "Core/JSONNode.hpp"
#include "Render/ETRenderInterfaces.hpp"

class TestUIBox : public UIBox {
public:
    virtual ~TestUIBox() = default;

    void setStyle(const UIStyle& uiStyle) {
        style = uiStyle;
    }

protected:

    bool serialize(const JSONNode& node) override { return true; }
};

class TestUIList : public UIList {
public:
    virtual ~TestUIList() = default;

    void setStyle(const UIStyle& uiStyle) {
        style = uiStyle;
    }

    void addElem(EntityId entId) {
        UIList::addElement(entId);
    }

protected:

    bool serialize(const JSONNode& node) override { return true; }
};

void UITests::SetUp() {
}

void UITests::TearDown() {
    tempObject.clear();
}

TestUIList* UITests::createUIList() {
    auto object = createVoidObject();
    std::unique_ptr<TestUIList> uiListPtr(new TestUIList);
    TestUIList* uiList = uiListPtr.get();
    object->addLogic(std::move(uiListPtr));
    tempObject.push_back(std::move(object));
    return uiList;
}

TestUIBox* UITests::createUIBox() {
    auto object = createVoidObject();
    std::unique_ptr<TestUIBox> uiBoxPtr(new TestUIBox);
    TestUIBox* uiBox = uiBoxPtr.get();
    object->addLogic(std::move(uiBoxPtr));
    tempObject.push_back(std::move(object));
    return uiBox;
}

TEST_F(UITests, CheckUIBoxSize) {
    TestUIBox* uiBox = createUIBox();

    UIStyle style;
    style.size = Vec2(0.7f, 0.7f);
    style.alignType = AlignType::Center;
    uiBox->setStyle(style);
    ASSERT_TRUE(uiBox->init());

    auto& box = uiBox->ET_getAaabb2di();
    Vec2i renderPort(0);
    ET_SendEventReturn(renderPort, &ETRender::ET_getRenderPort);
    ASSERT_EQ(box.getCenter(), renderPort / 2);

    float shiftX = (1.f - style.size.x) / 2.f;
    float shiftY = (1.f - style.size.y) / 2.f;

    Vec2i expectedBot = Vec2i(static_cast<int>(renderPort.x * shiftX),
        static_cast<int>(renderPort.y * shiftY));
    ASSERT_EQ(box.bot, expectedBot);

    Vec2i expectedTop = Vec2i(static_cast<int>(renderPort.x * (style.size.x + shiftX)), 
        static_cast<int>(renderPort.y * (style.size.y + shiftX)));
    ASSERT_EQ(box.top, expectedTop);
}

TEST_F(UITests, CheckUIBoxInsideUIBox) {
    TestUIBox* uiBox1 = createUIBox();
    TestUIBox* uiBox2 = createUIBox();

    ET_SendEvent(uiBox1->getEntityId(), &ETGameObject::ET_addChild, uiBox2->getEntityId());

    UIStyle style;
    style.size = Vec2(0.5f, 0.5f);
    style.alignType = AlignType::Center;

    uiBox1->setStyle(style);
    ASSERT_TRUE(uiBox1->init());

    uiBox2->setStyle(style);
    ASSERT_TRUE(uiBox2->init());

    auto& parentBox = uiBox1->ET_getAaabb2di();
    auto parentSize = parentBox.getSize();
    auto& box = uiBox2->ET_getAaabb2di();

    ASSERT_EQ(box.getCenter(), parentBox.getCenter());
    float shiftX = (1.f - style.size.x) / 2.f;
    float shiftY = (1.f - style.size.y) / 2.f;

    Vec2i expectedBot = Vec2i(static_cast<int>(parentBox.bot.x + parentSize.x * shiftX),
        static_cast<int>(parentBox.bot.y + parentSize.y * shiftY));
    ASSERT_EQ(box.bot, expectedBot);

    Vec2i expectedTop = Vec2i(static_cast<int>(parentBox.bot.x + parentSize.x * (style.size.x + shiftX)), 
        static_cast<int>(parentBox.bot.y + parentSize.y * (style.size.y + shiftX)));
    ASSERT_EQ(box.top, expectedTop);
}

TEST_F(UITests, CheckVerticalUIList) {
    TestUIList* uiList = createUIList();
    UIStyle listStyle;
    listStyle.alignType = AlignType::Center;
    listStyle.listType = ListType::Vertical;
    uiList->setStyle(listStyle);

    UIStyle boxStyle;
    boxStyle.size = Vec2(0.5f, 0.5f);
    boxStyle.alignType = AlignType::Center;

    TestUIBox* uiBox1 = createUIBox();
    ASSERT_TRUE(uiBox1->init());

    TestUIBox* uiBox2 = createUIBox();
    ASSERT_TRUE(uiBox2->init());

    uiList->addElem(uiBox1->getEntityId());
    uiList->addElem(uiBox2->getEntityId());

    ASSERT_TRUE(uiList->init());
}