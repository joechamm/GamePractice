#ifndef __THREAD_JOB_HPP__
#define __THREAD_JOB_HPP__

#include <atomic>
#include <vector>

class RunTask;
class JobTree;
class TimePoint;

class ThreadJob {
public:

    ThreadJob(RunTask* runTask);
    ~ThreadJob();

    bool canStartInThread(int threadId) const;
    void scheduleNextJobs(std::vector<ThreadJob*>& output);
    std::vector<ThreadJob*>& getNextJobs();
    std::vector<ThreadJob*>& getChildJobs();
    void setTree(JobTree* tree);
    JobTree* getTree();
    void setParentsCount(int newParentCount);
    int getParentsCount() const;
    int getRunCount() const;
    void addChildJob(ThreadJob* childJob);
    RunTask* getTask();
    void execute(const TimePoint& currTime);

private:

    void onFinished(const TimePoint& currTime);
    void onParentTaskFinished();

private:

    std::vector<ThreadJob*> childrenJobs;
    std::vector<ThreadJob*>* nextJobs;
    std::atomic<int> pendingParents;
    RunTask* task;
    JobTree* tree;
    int parentsCount;
};

#endif /* __THREAD_JOB_HPP__ */