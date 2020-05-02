#ifndef __GAME_BOARD_LOGIC_HPP__
#define __GAME_BOARD_LOGIC_HPP__

#include "Entity/EntityLogic.hpp"
#include "Math/AABB.hpp"
#include "ETApplicationInterfaces.hpp"
#include "Game/ETGameInterfaces.hpp"
#include "UI/ETUIInterfaces.hpp"
#include "Entity/ETEntityInterfaces.hpp"

#include <vector>
#include <memory>

struct BoardElement {
    AABB2Di box;
    Vec2i movePt;
    Vec2i boardPt;
    EntityId entId;
    EBoardElemType type;
    EBoardElemState state;
};

class GameBoardLogic : public EntityLogic,
    public ETNode<ETGameTimerEvents>,
    public ETNode<ETUIBoxEvents>,
    public ETNode<ETEntityEvents>,
    public ETNode<ETGameBoard> {
public:

    static void Reflect(ReflectContext& ctx);

public:

    GameBoardLogic();
    virtual ~GameBoardLogic();

    // EntityLogic
    bool init() override;
    void deinit() override;

    // ETGameBoard
    void ET_switchElemsBoardPos(EntityId firstId, EntityId secondId) override;
    void ET_setElemState(EntityId elemEntId, EBoardElemState newState) override;
    EBoardElemState ET_getElemState(EntityId elemEntId) const override;
    EBoardElemType ET_getElemType(EntityId elemEntId) const override;
    void ET_updateBoard() override;
    EntityId ET_getElemByPos(const Vec2i& pt) const override;
    EntityId ET_getElemByBoardPos(const Vec2i& boardPt) const override;
    Vec2i ET_getElemBoardPos(EntityId elemEntId) const override;
    int ET_getCellSize() const override;
    const Vec2i& ET_getBoardSize() const override;
    const AABB2Di& ET_getBoardBox() const override;
    Vec3 ET_getPosFromBoardPos(const Vec2i& boardPt) const override;

    // ETGameTimerEvents
    void ET_onGameTick(float dt) override;

    // ETUIBoxEvents
    void ET_onBoxResized() override;

    // ETEntityEvents
    void ET_onTransformChanged(const Transform& newTm) override;
    void ET_onChildAdded(EntityId childId) override { (void)childId; }

protected:

    int getElemId(const Vec2i& boardPt) const;
    Vec2i getBoardPosFromPos(const Vec2i& boardPt, const Vec3& pt) const;
    const BoardElement* getElem(EntityId entId) const;
    BoardElement* getElem(EntityId entId);
    BoardElement* getElem(const Vec2i& boardPt);
    const BoardElement* getElem(const Vec2i& boardPt) const;
    int getVoidElemBelow(const Vec2i& boardPt) const;
    const BoardElement* getTopElem(const Vec2i& boardPt) const;
    bool moveElem(BoardElement& elem, float dt);
    void initNewElem(BoardElement& elem, const Vec2i& boardPt) const;
    void setElemBoardPos(BoardElement& elem, const Vec2i& boardPt) const;
    void switchElements(int firstElem, int secondElem);
    ColorB getElemColor(EBoardElemType elemType) const;
    void updateAfterRemoves();
    void updateBoard();
    void initBoardBox();

protected:

    virtual void setElemType(BoardElement& elem) const;

protected:

    std::vector<BoardElement> elements;
    std::string cellObject;
    AABB2Di boardBox;
    Vec2i boardSize;
    Vec2i objectSize;
    float cellScale;
    float moveSpeed;
    int cellSize;
    int activeTouchedElemId;
    bool doUpdate;
};

#endif /* __GAME_BOARD_LOGIC_HPP__ */