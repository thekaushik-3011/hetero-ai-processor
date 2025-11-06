//============================================================================
// File: tensor_core.h
// Description: Tensor/AI core with systolic array
//============================================================================

#ifndef TENSOR_CORE_H
#define TENSOR_CORE_H

#include "common_types.h"
#include <queue>

class TensorCore {
public:
    TensorCore(int id = 0, int array_size = 8);
    ~TensorCore();
    
    // Simulation interface
    void clock();
    void reset();
    
    // Task interface
    bool submitTask(const TaskDescriptor& task);
    bool isIdle() const { return idle_; }
    bool isBusy() const { return !idle_; }
    
    // Performance counters
    uint64_t getCycleCount() const { return cycle_count_; }
    uint64_t getTaskCount() const { return task_count_; }
    uint64_t getBusyCycles() const { return busy_cycles_; }
    uint64_t getMACOperations() const { return mac_operations_; }
    
    // Configuration
    int getArraySize() const { return array_size_; }
    int getCoreId() const { return core_id_; }
    
private:
    // Core configuration
    int core_id_;
    int array_size_;  // e.g., 8 for 8x8 systolic array
    
    // Task queue
    std::queue<TaskDescriptor> task_queue_;
    static constexpr int MAX_QUEUE_DEPTH = 16;
    
    // Performance counters
    uint64_t cycle_count_;
    uint64_t task_count_;
    uint64_t busy_cycles_;
    uint64_t mac_operations_;
    bool idle_;
    
    // Current task execution
    TaskDescriptor current_task_;
    int execution_cycles_remaining_;
    
    // Task execution
    void executeMatrixMul();
    void executeConv2D();
    
    // Helper methods
    int estimateTaskCycles(const TaskDescriptor& task) const;
    int calculateTiles(int dimension) const;
};

#endif // TENSOR_CORE_H
