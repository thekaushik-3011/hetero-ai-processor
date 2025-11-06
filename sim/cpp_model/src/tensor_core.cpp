#include "tensor_core.h"
#include <iostream>

TensorCore::TensorCore(int id, int array_size)
    : core_id_(id), array_size_(array_size),
      cycle_count_(0), task_count_(0), busy_cycles_(0), 
      mac_operations_(0), idle_(true), execution_cycles_remaining_(0) {
    
    std::cout << "[TensorCore" << core_id_ << "] Initialized with " 
              << array_size_ << "x" << array_size_ << " systolic array" << std::endl;
}

TensorCore::~TensorCore() {
}

void TensorCore::reset() {
    while (!task_queue_.empty()) task_queue_.pop();
    cycle_count_ = 0;
    task_count_ = 0;
    busy_cycles_ = 0;
    mac_operations_ = 0;
    idle_ = true;
    execution_cycles_remaining_ = 0;
}

bool TensorCore::submitTask(const TaskDescriptor& task) {
    if (task_queue_.size() >= MAX_QUEUE_DEPTH) {
        return false;  // Queue full
    }
    
    task_queue_.push(task);
    return true;
}

void TensorCore::clock() {
    cycle_count_++;
    
    // Check if we can start a new task
    if (idle_ && !task_queue_.empty()) {
        current_task_ = task_queue_.front();
        task_queue_.pop();
        
        execution_cycles_remaining_ = estimateTaskCycles(current_task_);
        idle_ = false;
        task_count_++;
        
        std::cout << "[TensorCore" << core_id_ << "] Starting task, estimated " 
                  << execution_cycles_remaining_ << " cycles" << std::endl;
    }
    
    // Execute current task
    if (!idle_) {
        busy_cycles_++;
        execution_cycles_remaining_--;
        
        // Count MAC operations per cycle (peak = array_size^2)
        mac_operations_ += array_size_ * array_size_;
        
        if (execution_cycles_remaining_ <= 0) {
            std::cout << "[TensorCore" << core_id_ << "] Task completed" << std::endl;
            idle_ = true;
        }
    }
}

int TensorCore::estimateTaskCycles(const TaskDescriptor& task) const {
    // Simple cycle estimation for Week 1
    switch (task.type) {
        case TaskType::MATRIX_MUL: {
            int m_tiles = calculateTiles(task.dim_m);
            int n_tiles = calculateTiles(task.dim_n);
            int k_tiles = calculateTiles(task.dim_k);
            // Each tile takes array_size cycles to compute
            return m_tiles * n_tiles * k_tiles * array_size_ + 50;  // +overhead
        }
        case TaskType::CONV2D:
            return 500;  // Placeholder
        default:
            return 1000;
    }
}

int TensorCore::calculateTiles(int dimension) const {
    return (dimension + array_size_ - 1) / array_size_;  // Ceiling division
}

void TensorCore::executeMatrixMul() {
    // TODO: Implement in Week 3
}

void TensorCore::executeConv2D() {
    // TODO: Implement in Week 3
}
