//============================================================================
// File: common_types.h
// Description: Common type definitions for the simulator
//============================================================================

#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <cstdint>
#include <string>
#include <vector>

// Task types
enum class TaskType {
    VECTOR_ADD = 0,
    VECTOR_MUL,
    VECTOR_FMA,
    MATRIX_MUL,
    CONV2D,
    ACTIVATION,
    UNKNOWN
};

// Core types
enum class CoreType {
    VECTOR_CORE = 0,
    TENSOR_CORE,
    AUTO_SELECT
};

// Task descriptor structure (64 bytes)
struct TaskDescriptor {
    TaskType type;
    CoreType preferred_core;
    uint64_t src_addr;
    uint64_t dst_addr;
    uint32_t dim_m;
    uint32_t dim_n;
    uint32_t dim_k;
    uint32_t priority;
    uint32_t flags;
    uint32_t reserved[7];  // Pad to 64 bytes
    
    TaskDescriptor() : type(TaskType::UNKNOWN), preferred_core(CoreType::AUTO_SELECT),
                       src_addr(0), dst_addr(0), dim_m(0), dim_n(0), dim_k(0),
                       priority(0), flags(0) {
        for (int i = 0; i < 7; i++) reserved[i] = 0;
    }
    
    std::string toString() const;
};

// Performance statistics
struct PerfStats {
    uint64_t total_cycles;
    uint64_t vector_core_cycles;
    uint64_t tensor_core_cycles;
    uint64_t vector_core_tasks;
    uint64_t tensor_core_tasks;
    uint64_t total_tasks;
    
    double vector_utilization() const {
        return total_cycles > 0 ? (double)vector_core_cycles / total_cycles : 0.0;
    }
    
    double tensor_utilization() const {
        return total_cycles > 0 ? (double)tensor_core_cycles / total_cycles : 0.0;
    }
    
    void reset() {
        total_cycles = 0;
        vector_core_cycles = 0;
        tensor_core_cycles = 0;
        vector_core_tasks = 0;
        tensor_core_tasks = 0;
        total_tasks = 0;
    }
};

#endif // COMMON_TYPES_H
