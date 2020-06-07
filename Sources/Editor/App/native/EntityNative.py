from .Native import NativeObject
from .LogicNative import CreateLogic

class EntityNative(NativeObject):
    def __init__(self):
        self._isModified = False
        self._name = None
        self._entityId = None
        self._tmLogic = None
        self._children = []
        self._logics = []
        self._parent = None
        self._childId = None

    def getName(self):
        return self._name

    def isModified(self):
        if self._isModified:
            return True
        if self._tmLogic.isModified():
            return True
        for logic in self._logics:
            if logic.isModified():
                return True
        for child in self._children:
            if child.isModified():
                return True
        return False

    def isLoadedToNative(self):
        return self._entityId is not None

    def _syncWithNative(self):
        for childEnt in self._children:
            childEnt._entityId = self._getAPI().getLibrary().getEntityChildEntityId(self._entityId, childEnt._childId)
            childEnt._syncWithNative()
        for logic in self._logics:
            logic.readFromNative()

    def _desyncWithNative(self):
        self._entityId = None
        for childEnt in self._children:
            childEnt._desyncWithNative()

    def loadToNative(self):
        self._entityId = self._getAPI().getLibrary().loadEntity(self._name)
        if self._entityId == 0:
            print("[EntityNative:loadToNative] Can't load entity '{0}' to editor".format(self._name))
            return False
        self._syncWithNative()
        return True

    def unloadFromNative(self):
        self._getAPI().getLibrary().unloadEntity(self._entityId)
        self._entityId = None

    def addLogic(self, logicName):
        if not self.isLoadedToNative():
            raise RuntimeError("Can't add logic '{0}' to entity that is not loaded to edit: '{1}'".format(
                logicName, self._name))
        logicId = self._getAPI().getLibrary().addLogicToEntity(self._entityId, logicName)
        if logicId == -1:
            print("[EntityNative:addLogic] Can't create native part of logic '{0}' for entity '{1}'".format(
                logicName, self._name))
            return None
        logic = CreateLogic(logicName)
        if logic is None:
            print("[EntityNative:addLogic] Can't add '{0}' logic to entity '{1}'".format(logicName, self._name))
            return None
        logic._logicId = logicId
        logic._entity = self
        logic.readFromNative()
        self._logics.append(logic)
        self._isModified = True
        return logic

    def addLogicWithData(self, logicName, logicId, logicData):
        if self.isLoadedToNative():
            raise RuntimeError("Can't add logic '{0}' with data to entity that is loaded to editor: '{1}'".format(
                logicName, self._name))
        logic = CreateLogic(logicName)
        if logic is None:
            print("[EntityNative:addLogicWithData] Can't add '{0}' logic to entity '{1}'".format(logicName, self._name))
            return None
        logic._logicId = logicId
        logic._entity = self
        logic._rootValue.readFromDict(logicData)
        self._logics.append(logic)
        return logic

    def initTransformLogic(self, logicData):
        if self.isLoadedToNative():
            raise RuntimeError("Can't add transform to entity that is loaded to editor: '{9}'".format(self._name))
        if self._tmLogic is not None:
            raise RuntimeError("Transform logic already exists for an entity: {0}".format(self._name))
        self._tmLogic = CreateLogic("Transform")
        self._tmLogic._logicId = 0
        self._tmLogic._entity = self
        self._tmLogic._rootValue.readFromDict(logicData)

    def removeLogic(self, logicId):
        if not self.isLoadedToNative():
            raise RuntimeError("Can't remove logic from entity that isn't loaded to native: '{0}'", self._name)
        logicToRemove = None
        for logic in self._logics:
            if logic.getNativeId() == logicId:
                logicToRemove = logic
        if logicToRemove is None:
            print("[EntityNative:removeLogic] Can't find logic to remove with id '{0}' from entity '{1}'".format(
                logicId, self._name))
            return
        self._getAPI().getLibrary().removeLogicFromEntity(self._entityId, logicId)
        self._logics.remove(logicToRemove)
        self._isModified = True

    def getLogics(self):
        return self._logics

    def getChildren(self):
        return self._children

    def getNativeId(self):
        return self._entityId

    def addChildEntity(self, entityName):
        if not self.isLoadedToNative():
            raise RuntimeError("Can't add child '{0}' entity to entity '{1}' that is not loaded to edit".format(
                entityName, self._name))
        childEntity = self._getAPI().getEntityLoader().loadEntity(entityName)
        if childEntity is None:
            print("[EntityNative:addChildEntity] Can't load entity form file '{0}' to add as a child to '{1}'".format(entityName, self._name))
            return None
        if not childEntity.loadToNative():
            print("[EntityNative:addChildEntity] Can't load entity '{0}' to native to add as a child to '{1}'".format(entityName, self._name))
            return None
        childEntity._childId = self._getAPI().getLibrary().addChildEntityToEntity(self._entityId, childEntity._entityId)
        if childEntity._childId == -1:
            childEntity.unloadFromNative()
            print("[EntityNative:addChildEntity] Can't add child entity '{0}' to entity: '{1}'".format(entityName, self._name))
            return None
        childEntity._parent = self
        self._children.append(childEntity)
        self._isModified = True
        return childEntity

    def removeChildEntity(self, childEntity):
        if not self.isLoadedToNative():
            raise RuntimeError("Can't remove child entity with id '{0}' from entity '{1}' that isn't loaded to edit".format(
                childEntity._name, self._name))
        childToRemove = None
        for child in self._children:
            if child.getNativeId() == childEntity.getNativeId():
                childToRemove = child
                break
        if childToRemove is None:
            print("[EntityNative:removeChildEntity] Can't find child '{0}' with id '{1}' to remove from entity {2}".format(
                childEntity._name, childEntity.getNativeId(), self._name))
            return
        self._getAPI().getLibrary().removeChildEntityFromEntity(self._entityId, childEntity.getNativeId())
        childToRemove.unloadFromNative()
        self._children.remove(childToRemove)
        self._isModified = True

    def getFullFilePath(self):
        return self._getAPI().getEntityLoader().getEntityFullPath(self._name)

    def dumpToDict(self):
        res = {"transform":{}, "children":[], "logics":[]}
        tmRes = {}
        self._tmLogic.writeToDict(tmRes)
        res["transform"] = tmRes["data"]
        for child in self._children:
            res["children"].append({
                "name": child.getName(),
                "id": child._childId
            })
        for logic in self._logics:
            logicRes = {}
            logic.writeToDict(logicRes)
            res["logics"].append(logicRes)
        return res

    def getTransformLogic(self):
        return self._tmLogic

    def save(self):
        self._getAPI().getEntityLoader().saveEntity(self)
        for child in self._children:
            child.save()
        self._isModified = False

    def canAddChild(self, entityName):
        if self._name == entityName:
            return False
        if self._parent is not None:
            return self._parent.canAddChild(entityName)
        return True