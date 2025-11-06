//============================================================================
// File: scheduler.h
// Description: Hardware scheduler for task dispatch
//============================================================================

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "common_types.h"
#include "vector_core.h"
#include "tensor_core.h"
#include <memory>
#include <queue>

class Scheduler {
public:
    Scheduler();
    ~Scheduler();
    
    // Initialize with cores
    void initialize(VectorCore* vector_core, TensorCore* tensor_core);
    
    // Simulation interface
    void clock();
    void reset();
    
    // Task submission
    bool submitTask(const TaskDescriptor& task);
    
    // Performance statistics
    PerfStats getStats() const { return stats_; }
    int getQueueDepth() const { return static_cast<int>(task_queue_.size()); }
    
private:
    // Connected cores
    VectorCore* vector_core_;
    TensorCore* tensor_core_;
    
    // Task queue
    std::queue<TaskDescriptor> task_queue_;
    static constexpr int MAX_QUEUE_DEPTH = 32;
    
    // Performance statistics
    PerfStats stats_;
    
    // Scheduling methods
    CoreType selectCore(const TaskDescriptor& task);
    bool dispatchTask(const TaskDescriptor& task, CoreType core);
    
    // Heuristics (Week 1 baseline)
    CoreType simpleHeuristic(const TaskDescriptor& task);
};

#endif // SCHEDULER_
