#ifndef __GAME_SCORE_LOGIC_HPP__
#define __GAME_SCORE_LOGIC_HPP__

#include "Entity/EntityLogic.hpp"
#include "Game/ETGameInterfaces.hpp"
#include "Core/ETPrimitives.hpp"

class ReflectContext;

class GameScoreLogic : public EntityLogic,
    public ETNode<ETGameScore>,
    public ETNode<ETGameBoardElemDestoryEvents> {
public:

    static void Reflect(ReflectContext& ctx);

public:

    GameScoreLogic();
    virtual ~GameScoreLogic();

    // EntityLogic
    bool init() override;
    void deinit() override;

    // ETGameBoardElemDestoryEvents
    void ET_onElemsDestroyed(EntityId elemId) override;

    // ETGameScore
    void ET_resetScore() override;
    int ET_getGameScore() const override;

private:

    int score;
};

#endif /* __GAME_SCORE_LOGIC_HPP__ */