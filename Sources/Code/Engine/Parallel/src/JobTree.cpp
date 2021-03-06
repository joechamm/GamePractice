#include "JobTree.hpp"

#include <cassert>

JobTree::JobTree() :
    pendingJobsCount(0),
    runDelay(0.f),
    jobsCount(0) {
}

JobTree::~JobTree() {
}

bool JobTree::tryFinishTreeByOneJob(const TimePoint& currTime) {
    auto remaimingJobs = pendingJobsCount.fetch_sub(1) - 1;
    if(!remaimingJobs) {
        prevTickT = currTime;
        pendingJobsCount.store(jobsCount);
        return true;
    }
    return false;
}

std::vector<ThreadJob*>& JobTree::getRootJobs() {
    return rootJobs;
}

void JobTree::addRootJob(ThreadJob* job) {
    rootJobs.push_back(job);
}

int JobTree::getJobsCount() const {
    return jobsCount;
}

void JobTree::setJobsCount(int newJobsCount) {
    jobsCount = newJobsCount;
    pendingJobsCount.store(jobsCount);
}

bool JobTree::isDelayPassed(const TimePoint& currTime) const {
    return currTime.getSecondsElapsedFrom(prevTickT) >= runDelay;
}

void JobTree::setRunDelay(float newRunDelay) {
    runDelay = newRunDelay;
}