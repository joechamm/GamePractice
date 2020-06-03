#ifndef __GAME_BOARD_TESTS_HPP__
#define __GAME_BOARD_TESTS_HPP__

#include "TestUtils/ConsoleAppTests.hpp"

class TestGameBoardLogic;
class Entity;

class GameBoardTests : public ConsoleAppTests {
protected:

    void SetUp();

protected:

    Entity* entity;
    TestGameBoardLogic* board;
};

#endif /* __GAME_BOARD_TESTS_HPP__ */