#ifndef __GAME_BOARD_ELEM_LOGIC_HPP__
#define __GAME_BOARD_ELEM_LOGIC_HPP__

#include "Entity/EntityLogic.hpp"
#include "Game/ETGameElem.hpp"
#include "Core/ETPrimitives.hpp"

class ReflectContext;

class GameBoardElemLogic : public EntityLogic,
    public ETNode<ETGameBoardElem>,
    public ETNode<ETBoardElemDetroyAnimationEvents> {
public:

    static void Reflect(ReflectContext& ctx);

public:

    GameBoardElemLogic();
    virtual ~GameBoardElemLogic();

    // EntityLogic
    bool init() override;
    void deinit() override;

    // ETGameBoardElem
    void ET_setMoveState(EBoardElemMoveState newState) override;
    EBoardElemMoveState ET_getMoveState() const override;
    void ET_setLifeState(EBoardElemLifeState newState) override;
    EBoardElemLifeState ET_getLifeState() const override;
    void ET_setType(EBoardElemType newType) override;
    EBoardElemType ET_getType() const override;
    void ET_triggerDestroy() override;
    void ET_setSelected(bool flag) override;
    bool ET_canMatch() const override;
    bool ET_canSwitch() const override;

    // ETBoardElemDetroyAnimationEvents
    void ET_onDestryAnimEnded() override; 

private:

    EBoardElemLifeState lifeState;
    EBoardElemMoveState moveState;
    EBoardElemType type;
};

#endif /* __GAME_BOARD_ELEMENT_LOGIC_HPP__ */