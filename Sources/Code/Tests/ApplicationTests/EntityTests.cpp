#include "EntityTests.hpp"
#include "Entity/Entity.hpp"

namespace {

const char* TEST_OBJECT_NAME = "Game/Simple.json";

} // namespace

TEST_F(EntityTests, CreateSimpleEntity) {
    EntityId objId = InvalidEntityId;
    ET_SendEventReturn(objId, &ETEntityManager::ET_createEntity, TEST_OBJECT_NAME);
    ASSERT_NE(objId, InvalidEntityId);

    std::string name;
    ET_SendEventReturn(name, objId, &ETEntity::ET_getName);
    ASSERT_FALSE(name.empty());
}

TEST_F(EntityTests, CreateTwoSimpleEntities) {
    EntityId objId1 = InvalidEntityId;
    ET_SendEventReturn(objId1, &ETEntityManager::ET_createEntity, TEST_OBJECT_NAME);

    EntityId objId2 = InvalidEntityId;
    ET_SendEventReturn(objId2, &ETEntityManager::ET_createEntity, TEST_OBJECT_NAME);

    ASSERT_NE(objId1, InvalidEntityId);
    ASSERT_NE(objId2, InvalidEntityId);
    ASSERT_NE(objId1, objId2);

    std::string name1;
    ET_SendEventReturn(name1, objId1, &ETEntity::ET_getName);

    std::string name2;
    ET_SendEventReturn(name2, objId2, &ETEntity::ET_getName);

    ASSERT_EQ(name1, name1);
}

TEST_F(EntityTests, CreateInvalidEntity) {
    EntityId objId = InvalidEntityId;
    ET_SendEventReturn(objId, &ETEntityManager::ET_createEntity, "");
    ASSERT_EQ(objId, InvalidEntityId);
}

TEST_F(EntityTests, CreateParentWithChildEntity) {
    EntityId objId1 = InvalidEntityId;
    ET_SendEventReturn(objId1, &ETEntityManager::ET_createEntity, TEST_OBJECT_NAME);

    EntityId objId2 = InvalidEntityId;
    ET_SendEventReturn(objId2, &ETEntityManager::ET_createEntity, TEST_OBJECT_NAME);

    ET_SendEvent(objId2, &ETEntity::ET_setParent, objId1);

    EntityId parentId;
    ET_SendEventReturn(parentId, objId2, &ETEntity::ET_getParentId);

    ASSERT_EQ(parentId, objId1);

    ET_SendEvent(&ETEntityManager::ET_destroyEntity, objId1);

    ASSERT_FALSE(ET_IsExistNode<ETEntity>(objId1));
    ASSERT_FALSE(ET_IsExistNode<ETEntity>(objId2));
}

TEST_F(EntityTests, CheckChildInheritParentTransform) {
    EntityId objId1 = InvalidEntityId;
    ET_SendEventReturn(objId1, &ETEntityManager::ET_createEntity, TEST_OBJECT_NAME);

    EntityId objId2 = InvalidEntityId;
    ET_SendEventReturn(objId2, &ETEntityManager::ET_createEntity, TEST_OBJECT_NAME);

    ET_SendEvent(objId2, &ETEntity::ET_setParent, objId1);

    Vec3 offset = Vec3(0.5f);

    Transform childTm;
    ET_SendEventReturn(childTm, objId2, &ETEntity::ET_getTransform);
    childTm.pt += offset;
    ET_SendEvent(objId2, &ETEntity::ET_setTransform, childTm);

    Transform parentTm;
    ET_SendEventReturn(parentTm, objId1, &ETEntity::ET_getTransform);
    parentTm.pt += offset;
    ET_SendEvent(objId1, &ETEntity::ET_setTransform, parentTm);

    ET_SendEventReturn(childTm, objId2, &ETEntity::ET_getTransform);
    Vec3 diff = childTm.pt - parentTm.pt;

    ASSERT_FLOAT_EQ(diff.x, offset.x);
    ASSERT_FLOAT_EQ(diff.y, offset.x);
    ASSERT_FLOAT_EQ(diff.z, offset.x);
}

TEST_F(EntityTests, CheckChildInheritParentScale) {
    EntityId parentId;
    ET_SendEventReturn(parentId, &ETEntityManager::ET_createEntity, TEST_OBJECT_NAME);

    EntityId childId;
    ET_SendEventReturn(childId, &ETEntityManager::ET_createEntity, TEST_OBJECT_NAME);

    ET_SendEvent(childId, &ETEntity::ET_setParent, parentId);

    {
        Transform childTm;
        ET_SendEventReturn(childTm, childId, &ETEntity::ET_getTransform);
        childTm.scale = Vec3(0.5f);
        ET_SendEvent(childId, &ETEntity::ET_setTransform, childTm);
    }

    {
        Transform parentTm;
        ET_SendEventReturn(parentTm, parentId, &ETEntity::ET_getTransform);
        parentTm.scale = Vec3(0.5f);
        ET_SendEvent(parentId, &ETEntity::ET_setTransform, parentTm);
    }

    Transform tm;
    ET_SendEventReturn(tm, childId, &ETEntity::ET_getTransform);
    EXPECT_FLOAT_EQ(tm.scale.x, 0.25f);
    EXPECT_FLOAT_EQ(tm.scale.y, 0.25f);
    EXPECT_FLOAT_EQ(tm.scale.z, 0.25f);

    {
        Transform parentTm;
        ET_SendEventReturn(parentTm, parentId, &ETEntity::ET_getTransform);
        parentTm.scale = Vec3(1.f);
        ET_SendEvent(parentId, &ETEntity::ET_setTransform, parentTm);
    }

    ET_SendEventReturn(tm, childId, &ETEntity::ET_getTransform);
    EXPECT_FLOAT_EQ(tm.scale.x, 0.5f);
    EXPECT_FLOAT_EQ(tm.scale.y, 0.5f);
    EXPECT_FLOAT_EQ(tm.scale.z, 0.5f);
}

TEST_F(EntityTests, CheckExtendEntity) {
    ASSERT_TRUE(false);
}