#include "scheduler.h"
#include <iostream>

Scheduler::Scheduler()
    : vector_core_(nullptr), tensor_core_(nullptr) {
    stats_.reset();
    std::cout << "[Scheduler] Initialized" << std::endl;
}

Scheduler::~Scheduler() {
}

void Scheduler::initialize(VectorCore* vector_core, TensorCore* tensor_core) {
    vector_core_ = vector_core;
    tensor_core_ = tensor_core;
    std::cout << "[Scheduler] Cores connected" << std::endl;
}

void Scheduler::reset() {
    while (!task_queue_.empty()) task_queue_.pop();
    stats_.reset();
}

bool Scheduler::submitTask(const TaskDescriptor& task) {
    if (task_queue_.size() >= MAX_QUEUE_DEPTH) {
        return false;  // Queue full
    }
    
    task_queue_.push(task);
    stats_.total_tasks++;
    return true;
}

void Scheduler::clock() {
    stats_.total_cycles++;
    
    // Update core utilization
    if (vector_core_ && vector_core_->isBusy()) {
        stats_.vector_core_cycles++;
    }
    if (tensor_core_ && tensor_core_->isBusy()) {
        stats_.tensor_core_cycles++;
    }
    
    // Try to dispatch tasks
    if (!task_queue_.empty()) {
        TaskDescriptor task = task_queue_.front();
        CoreType selected_core = selectCore(task);
        
        if (dispatchTask(task, selected_core)) {
            task_queue_.pop();
            
            if (selected_core == CoreType::VECTOR_CORE) {
                stats_.vector_core_tasks++;
            } else {
                stats_.tensor_core_tasks++;
            }
        }
    }
}

CoreType Scheduler::selectCore(const TaskDescriptor& task) {
    // Week 1 baseline: simple heuristic
    return simpleHeuristic(task);
}

CoreType Scheduler::simpleHeuristic(const TaskDescriptor& task) {
    // Route based on task type
    switch (task.type) {
        case TaskType::MATRIX_MUL:
        case TaskType::CONV2D:
            return CoreType::TENSOR_CORE;
            
        case TaskType::VECTOR_ADD:
        case TaskType::VECTOR_MUL:
        case TaskType::VECTOR_FMA:
            return CoreType::VECTOR_CORE;
            
        default:
            // Check which core is idle
            if (vector_core_ && vector_core_->isIdle()) {
                return CoreType::VECTOR_CORE;
            } else if (tensor_core_ && tensor_core_->isIdle()) {
                return CoreType::TENSOR_CORE;
            } else {
                return CoreType::VECTOR_CORE;  // Default fallback
            }
    }
}

bool Scheduler::dispatchTask(const TaskDescriptor& task, CoreType core) {
    if (core == CoreType::VECTOR_CORE && vector_core_) {
        return vector_core_->submitTask(task);
    } else if (core == CoreType::TENSOR_CORE && tensor_core_) {
        return tensor_core_->submitTask(task);
    }
    return false;
}
