#include "Game/Logics/GameBoardElemDestroyLogic.hpp"
#include "Entity/ETEntity.hpp"
#include "Reflect/ReflectContext.hpp"

#include <algorithm>

GameBoardElemDestroyLogic::GameBoardElemDestroyLogic() {
}

GameBoardElemDestroyLogic::~GameBoardElemDestroyLogic() {
}

void GameBoardElemDestroyLogic::Reflect(ReflectContext& ctx) {
    if(auto classInfo = ctx.classInfo<GameBoardElemDestroyLogic>("GameBoardElemDestroy")) {
        classInfo->addField("destroyDuration", &GameBoardElemDestroyLogic::destroyDuration);
    }
}

bool GameBoardElemDestroyLogic::init() {
    ETNode<ETGameTimerEvents>::connect(getEntityId());
    ETNode<ETGameBoardElemDestroy>::connect(getEntityId());
    return true;
}

void GameBoardElemDestroyLogic::deinit() {
}

void GameBoardElemDestroyLogic::ET_onGameTick(float dt) {
    for(auto& task : destroyTasks) {
        task.duration += dt;
        float prog = std::min(task.duration / destroyDuration, 1.f);
        
        Transform tm;
        ET_SendEventReturn(tm, task.entId, &ETEntity::ET_getTransform);
        tm.scale = Math::Lerp(task.startScale, Vec3(0.f), prog);

        ET_SendEvent(task.entId, &ETEntity::ET_setTransform, tm);
    }

    int destroyedCount = 0;
    auto it = destroyTasks.begin();
    while(it != destroyTasks.end()) {
        auto& task = *it;
        if(task.duration > destroyDuration) {
            ET_SendEvent(getEntityId(), &ETGameBoard::ET_setElemState, task.entId, EBoardElemState::Void);
            it = destroyTasks.erase(it);
            ++destroyedCount;
        } else {
            ++it;
        }
    }

    if(destroyedCount > 0) {
        ET_SendEvent(getEntityId(), &ETGameBoard::ET_updateBoard);
        ET_SendEvent(getEntityId(), &ETGameBoardElemDestoryEvents::ET_onElemsDestroyed, destroyedCount);
    }
}

void GameBoardElemDestroyLogic::ET_destroyBoardElem(EntityId elemId) {
    ET_SendEvent(getEntityId(), &ETGameBoard::ET_setElemState, elemId, EBoardElemState::Removing);

    DestroyTask task;
    task.entId = elemId;
    task.duration = 0.f;

    Transform tm;
    ET_SendEventReturn(tm, task.entId, &ETEntity::ET_getTransform);
    task.startScale = tm.scale;

    destroyTasks.push_back(task);
}