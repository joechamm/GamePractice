from utils.Log import Log
from utils.MainViewManager import GetMainViewManager

from dialog.LogicSelecDialog import LogicSelectDialog
from dialog.SaveEntityChanges import SaveEntityChanges
from dialog.RemoveEntityLogic import RemoveEntityLogic
from dialog.OverrideFile import OverrideFile

from PyQt5.QtWidgets import QMessageBox, QFileDialog

import os

class _EventManager:

    _INSTANCE = None

    def __init__(self, app):
        if _EventManager._INSTANCE is not None:
            raise RuntimeError("One instance of event manager already exists")
        self._app = app
        self._currentEntity = None

    def _askToSaveEntity(self):
        if not self._currentEntity.isModified():
            return True
        retCode = SaveEntityChanges(self._currentEntity).exec_()
        if retCode == QMessageBox.Ok:
            self._currentEntity.save()
        elif retCode == QMessageBox.Cancel:
            return False
        return True

    def onEntityDoubleClickFromFileTree(self, entityName):
        if self._currentEntity is not None:
            if self._currentEntity.getName() == entityName:
                return
            if not self._askToSaveEntity():
                return
        loader = self._app._editorNative.getEntityLoader()
        try:
            newEntity = loader.loadEntity(entityName)
        except Exception as e:
            Log.error("[_EventManager:onEntityDoubleClickFromFileTree] Can't load entity '{0}' (Exception: {1})".format(
                entityName, e))
            return
        if newEntity is None:
            return
        if not newEntity.loadToNative():
            Log.error("[_EventManager:onEntityDoubleClickFromFileTree] Can't load entity '{0}' to native".format(
                entityName))
            return
        if self._currentEntity is not None:
            self._currentEntity.unloadFromNative()
        self._currentEntity = newEntity
        self._currentEntity._syncWithNative()
        GetMainViewManager().onEntityDoubleClickFromFileTree(self._currentEntity)

    def onEntityClickedFromEntityTree(self, entity):
        if entity is not None:
            entity._syncWithNative()
        GetMainViewManager().onEntityClickedFromEntityTree(entity)

    def onAddLogicBtClicked(self, editEntity):
        if editEntity is None:
            raise RuntimeError("Can't add logic to invlid entity")
        if not editEntity.isLoadedToNative():
            raise RuntimeError("Can't add logic to entity '{0}' that is not loaded to native".format(
                editEntity.getName()))
        selectDialog = LogicSelectDialog(self._app._logicsModel)
        selectDialog.exec_()
        logic = selectDialog.getResultLogic()
        if logic is None:
            return
        entityLogic = editEntity.addLogic(logic.getName())
        if entityLogic is None:
            raise RuntimeError("Can't add logic '{0}' to  entity '{1}'".format(logic.getName(), editEntity.getName()))
        self._app._entityLogicsView.addLogicView(entityLogic)

    def onRemoveEntityLogicBtClicked(self, editLogic):
        retCode = RemoveEntityLogic(editLogic).exec_()
        if retCode == QMessageBox.Ok:
            editLogic.getEntity().removeLogic(editLogic.getNativeId())
            self._app._entityLogicsView.removeLogicView(editLogic.getNativeId())
        elif retCode == QMessageBox.Cancel:
            pass
        elif retCode == QMessageBox.No:
            pass

    def onChildEntityExtractToFile(self, childEntity):
        name = childEntity.getFullFilePath()
        if name[-5:] != '.json':
            name += ".json"
        resFile = QFileDialog.getSaveFileName(self._app, "", "{0}".format(name), "Entity (*.json)")
        resFile = resFile[0]
        if resFile is None:
            return False
        if os.path.exists(resFile):
            if os.path.isdir(resFile):
                return False
            retCode = OverrideFile(resFile).exec_()
            if retCode != QMessageBox.Ok:
                return False
        if not childEntity.extractToFile(resFile):
            return False
        self._app._assetsModel.reload()
        GetMainViewManager().refreshEntityFileView()
        return True

    def getAssetsModel(self):
        return self._app._assetsModel

    def getSoundEventsModel(self):
        return self._app._soundEventsModel

    def hasEditEntity(self):
        return self._currentEntity is not None

    def closeEditEntity(self):
        if self._currentEntity is not None:
            if not self._askToSaveEntity():
                return
            self._currentEntity.unloadFromNative()
        self._currentEntity = None
        GetMainViewManager().closeEditEntity()

    def saveEditEntity(self):
        self._currentEntity.save()

def GetEventManager():
    return _EventManager._INSTANCE

def CreateEventManager(app):
    manger = _EventManager(app)
    _EventManager._INSTANCE = manger