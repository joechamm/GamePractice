#include "TaskRunnerTests.hpp"
#include "Parallel/TasksRunner.hpp"
#include "Parallel/RunTask.hpp"
#include "Core/TimePoint.hpp"

#include <thread>
#include <atomic>

TEST_F(TaskRunnerTests, SimpleOneTimeTaskOneThread) {
    TasksRunner runner;
    int value = 0;
    auto task = runner.createTask("Test", [&value](){
        ++value;
        std::this_thread::yield();
    });
    runner.runUntil(1, [&value](){
        return value < 1;
    });

    EXPECT_EQ(value, 1);
}

TEST_F(TaskRunnerTests, RunTwoTasksTwoThreads) {
    std::atomic<int> value = 0;
    TasksRunner runner;

    runner.createTask("Test1", [&value](){
        value.fetch_add(1);
        std::this_thread::yield();
    });
    runner.createTask("Test2", [&value](){
        value.fetch_add(1);
        std::this_thread::yield();
    });

    runner.runUntil(2, [&value](){
        return value.load() < 100;
    });

    EXPECT_GE(value.load(), 100);
    EXPECT_LE(value.load(), 110);
}

TEST_F(TaskRunnerTests, RunTenTasksTenThreads) {
    std::atomic<int> value = 0;
    TasksRunner runner;
    for(int i = 0; i < 10; ++i) {
        runner.createTask("Test", [&value](){
            value.fetch_add(1);
            std::this_thread::yield();
        });
    }
    runner.runUntil(10, [&value](){
        return value.load() < 10000;
    });

    int totalRunCount = 0;
    for(auto& task : runner.getTasks()) {
        EXPECT_GT(task->getRunCount(), 50);
        totalRunCount += task->getRunCount();
    }
    EXPECT_EQ(value.load(), totalRunCount);
    EXPECT_GE(value.load(), 10000);
    EXPECT_LE(value.load(), 10100);
}

TEST_F(TaskRunnerTests, TestFrequency) {
    int value = 0;

    TasksRunner runner;
    auto task = runner.createTask("Test", [&value](){
        ++value;
        std::this_thread::yield();
    });
    task->setFrequency(60);

    TimePoint startTime = TimePoint::GetNowTime();
    runner.runUntil(1, [&value, &startTime](){
        auto currTime = TimePoint::GetNowTime();
        return currTime.getSecondsElapsedFrom(startTime) < 1.f;
    });

    EXPECT_EQ(task->getRunCount(), 60);
    EXPECT_EQ(value, 60);
}

TEST_F(TaskRunnerTests, RunTaskWithChildren) {
    std::atomic<int> value(0);
    std::atomic<int> botValue(0);

    TasksRunner runner;

    auto topTask = runner.createTask("Top", [&value](){
        value.store(5);
    });
    auto leftTask = runner.createTask("Left", [&value](){
        value.fetch_sub(2);
    });
    auto rightTask = runner.createTask("Right", [&value](){
        value.fetch_sub(2);
    });
    auto botTask = runner.createTask("Bot", [&value, &botValue](){
        botValue.fetch_add(value.load());
    });

    topTask->addChild(leftTask);
    topTask->addChild(rightTask);
    leftTask->addChild(botTask);
    rightTask->addChild(botTask);

    runner.runUntil(4, [&botValue](){
        return botValue.load() < 500;
    });

    for(auto& task : runner.getTasks()) {
        EXPECT_EQ(task->getRunCount(), botValue.load());
    }
}