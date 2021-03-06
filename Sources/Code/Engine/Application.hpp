#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__

#include <memory>
#include <vector>

class SystemModule;
class ETSystem;

class Application {
public:

    using ETSystemPtrT = std::unique_ptr<ETSystem>;
    using ModuleListT = std::vector<std::unique_ptr<SystemModule>>;

public:

    Application();
    virtual ~Application();

    int run();

protected:

    bool init();
    void mainLoop();
    void deinit();

protected:

    virtual void buildModules(ModuleListT& modules);

private:

    Application(const Application& app) = delete;
    Application& operator=(const Application& app) = delete;

private:

    ETSystemPtrT etSystem;
    ModuleListT systemModules;
};

#endif /* __APPLICATION_HPP__ */