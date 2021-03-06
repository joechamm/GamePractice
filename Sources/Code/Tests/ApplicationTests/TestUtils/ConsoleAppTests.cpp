#include "TestUtils/ConsoleAppTests.hpp"

#include <Application.hpp>
#include <Render/RenderModule.hpp>
#include <Entity.hpp>
#include <Core/ETPrimitives.hpp>
#include <Entity/EntityModule.hpp>
#include <UI/UIModule.hpp>
#include <Game/GameModule.hpp>
#include <Platform/PlatformModule.hpp>
#include <Core/ETLogger.hpp>

namespace {

std::unique_ptr<Application> APP;

const char* TEST_OBJECT_NAME = "TestObject";

class ConsoleAppTest : public Application {
public:

    ConsoleAppTest() : Application() {}
    virtual ~ConsoleAppTest() = default;

    bool init() { return Application::init(); }

protected:

    void buildModules(ModuleListT& modules) override {
        modules.emplace_back(new EntityModule);
        modules.emplace_back(new PlatformModule);
        modules.emplace_back(new RenderModule);
        modules.emplace_back(new UIModule);
        modules.emplace_back(new GameModule);
    }
};

} // namespace

void ConsoleAppTests::SetUpTestCase() {
    ConsoleAppTest* consoleApp = new ConsoleAppTest();
    if (!consoleApp->init()) {
        delete consoleApp;
        ASSERT_TRUE(false);
    }
    APP.reset(consoleApp);
    ET_SendEvent(&ETLogger::ET_setLogLevel, LogLevel::Info);
}

void ConsoleAppTests::TearDownTestCase() {
    APP.reset();
}

void ConsoleAppTests::TearDown() {
    tempObject.clear();
}

Entity* ConsoleAppTests::createVoidObject() {
    std::unique_ptr<Entity> objectPtr(new Entity(TEST_OBJECT_NAME, GetETSystem()->createNewEntityId()));
    auto ptr = objectPtr.get();
    tempObject.push_back(std::move(objectPtr));
    return ptr;
}