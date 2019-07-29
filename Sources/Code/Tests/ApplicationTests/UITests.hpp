#ifndef __UI_TESTS_HPP__
#define __UI_TESTS_HPP__

#include "TestUtils/ConsoleAppTests.hpp"

#include <vector>

class UIBox;
class UIList;
class GameObject;

class UITests : public ConsoleAppTests {
protected:

    void SetUp();
    void TearDown();

    UIBox* createUIBox();
    UIList* createUIList();

protected:

    std::vector<std::unique_ptr<GameObject>> tempObject;
};

#endif /* __UI_TESTS_HPP__ */