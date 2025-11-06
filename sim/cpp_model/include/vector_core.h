//============================================================================
// File: vector_core.h
// Description: Vector/SIMD core simulator
//============================================================================

#ifndef VECTOR_CORE_H
#define VECTOR_CORE_H

#include "common_types.h"
#include <array>
#include <queue>

class VectorCore {
public:
    VectorCore(int id = 0, int num_lanes = 8);
    ~VectorCore();
    
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
    
    // Configuration
    int getNumLanes() const { return num_lanes_; }
    int getCoreId() const { return core_id_; }
    
private:
    // Core configuration
    int core_id_;
    int num_lanes_;
    
    // Register file (32 vector registers, each 256-bit / 8x32-bit elements)
    static constexpr int NUM_REGS = 32;
    static constexpr int ELEMENTS_PER_REG = 8;
    std::array<std::array<float, ELEMENTS_PER_REG>, NUM_REGS> register_file_;
    
    // Pipeline state
    enum class PipelineStage {
        IDLE,
        FETCH,
        DECODE,
        EXECUTE,
        WRITEBACK
    };
    PipelineStage current_stage_;
    
    // Task queue
    std::queue<TaskDescriptor> task_queue_;
    static constexpr int MAX_QUEUE_DEPTH = 16;
    
    // Performance counters
    uint64_t cycle_count_;
    uint64_t task_count_;
    uint64_t busy_cycles_;
    bool idle_;
    
    // Current task execution
    TaskDescriptor current_task_;
    int execution_cycles_remaining_;
    
    // Pipeline methods
    void pipelineFetch();
    void pipelineDecode();
    void pipelineExecute();
    void pipelineWriteback();
    
    // Task execution
    void executeVectorAdd();
    void executeVectorMul();
    void executeVectorFMA();
    
    // Helper methods
    int estimateTaskCycles(const TaskDescriptor& task) const;
};

#endif // VECTOR_CORE_H
