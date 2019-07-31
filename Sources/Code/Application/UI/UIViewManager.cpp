#include "UI/UIViewManager.hpp"
#include "Game/ETGameInterfaces.hpp"
#include "ETApplicationInterfaces.hpp"

UIViewManager::UIViewManager() {
}

UIViewManager::~UIViewManager() {
}

bool UIViewManager::init() {
    ETNode<ETUIViewManager>::connect(getEntityId());
    return true;
}

void UIViewManager::deinit() {
    ETNode<ETUIViewManager>::disconnect();
}

EntityId UIViewManager::ET_openView(const char* viewName) {
    if(!viewName || !viewName[0]) {
        LogWarning("[UIViewManager::ET_openView] Can't open empty view");
        return InvalidEntityId;
    }
    EntityId newViewId;
    ET_SendEventReturn(newViewId, &ETGameObjectManager::ET_createGameObject, viewName);
    if(!newViewId.isValid()) {
        LogWarning("[UIViewManager::ET_openView] Can't open view: %s", viewName);
        return InvalidEntityId;
    }

    ET_SendEvent(&ETGameObjectManager::ET_destroyObject, activeViewId);
    activeViewId = newViewId;
    return activeViewId;
}

void UIViewManager::ET_closeView(EntityId viewId) {
    if(activeViewId == viewId && activeViewId.isValid()) {
        ET_SendEvent(&ETGameObjectManager::ET_destroyObject, activeViewId);
        activeViewId = InvalidEntityId;
    }
}