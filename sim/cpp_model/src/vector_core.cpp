#include "vector_core.h"
#include <iostream>
#include <cstring>

VectorCore::VectorCore(int id, int num_lanes)
    : core_id_(id), num_lanes_(num_lanes), current_stage_(PipelineStage::IDLE),
      cycle_count_(0), task_count_(0), busy_cycles_(0), idle_(true),
      execution_cycles_remaining_(0) {
    
    // Initialize register file to zero
    for (auto& reg : register_file_) {
        reg.fill(0.0f);
    }
    
    std::cout << "[VectorCore" << core_id_ << "] Initialized with " 
              << num_lanes_ << " lanes" << std::endl;
}

VectorCore::~VectorCore() {
}

void VectorCore::reset() {
    while (!task_queue_.empty()) task_queue_.pop();
    current_stage_ = PipelineStage::IDLE;
    cycle_count_ = 0;
    task_count_ = 0;
    busy_cycles_ = 0;
    idle_ = true;
    execution_cycles_remaining_ = 0;
    
    for (auto& reg : register_file_) {
        reg.fill(0.0f);
    }
}

bool VectorCore::submitTask(const TaskDescriptor& task) {
    if (task_queue_.size() >= MAX_QUEUE_DEPTH) {
        return false;  // Queue full
    }
    
    task_queue_.push(task);
    return true;
}

void VectorCore::clock() {
    cycle_count_++;
    
    // Check if we can start a new task
    if (idle_ && !task_queue_.empty()) {
        current_task_ = task_queue_.front();
        task_queue_.pop();
        
        execution_cycles_remaining_ = estimateTaskCycles(current_task_);
        idle_ = false;
        task_count_++;
        
        std::cout << "[VectorCore" << core_id_ << "] Starting task, estimated " 
                  << execution_cycles_remaining_ << " cycles" << std::endl;
    }
    
    // Execute current task
    if (!idle_) {
        busy_cycles_++;
        execution_cycles_remaining_--;
        
        if (execution_cycles_remaining_ <= 0) {
            std::cout << "[VectorCore" << core_id_ << "] Task completed" << std::endl;
            idle_ = true;
        }
    }
}

int VectorCore::estimateTaskCycles(const TaskDescriptor& task) const {
    // Simple cycle estimation for Week 1
    switch (task.type) {
        case TaskType::VECTOR_ADD:
            return task.dim_m / num_lanes_ + 5;  // Vectorized + overhead
        case TaskType::VECTOR_MUL:
            return task.dim_m / num_lanes_ + 5;
        case TaskType::VECTOR_FMA:
            return (task.dim_m / num_lanes_) * 3 + 10;
        default:
            return 100;  // Unknown task
    }
}

void VectorCore::pipelineFetch() {
    // TODO: Implement in Week 2
}

void VectorCore::pipelineDecode() {
    // TODO: Implement in Week 2
}

void VectorCore::pipelineExecute() {
    // TODO: Implement in Week 2
}

void VectorCore::pipelineWriteback() {
    // TODO: Implement in Week 2
}

void VectorCore::executeVectorAdd() {
    // TODO: Implement in Week 2
}

void VectorCore::executeVectorMul() {
    // TODO: Implement in Week 2
}

void VectorCore::executeVectorFMA() {
    // TODO: Implement in Week 2
}
