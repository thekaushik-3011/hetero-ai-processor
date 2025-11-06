#include "common_types.h"
#include <sstream>

std::string TaskDescriptor::toString() const {
    std::stringstream ss;
    ss << "Task{type=";
    
    switch (type) {
        case TaskType::VECTOR_ADD: ss << "VECTOR_ADD"; break;
        case TaskType::VECTOR_MUL: ss << "VECTOR_MUL"; break;
        case TaskType::VECTOR_FMA: ss << "VECTOR_FMA"; break;
        case TaskType::MATRIX_MUL: ss << "MATRIX_MUL"; break;
        case TaskType::CONV2D: ss << "CONV2D"; break;
        case TaskType::ACTIVATION: ss << "ACTIVATION"; break;
        default: ss << "UNKNOWN"; break;
    }
    
    ss << ", core=";
    switch (preferred_core) {
        case CoreType::VECTOR_CORE: ss << "VECTOR"; break;
        case CoreType::TENSOR_CORE: ss << "TENSOR"; break;
        case CoreType::AUTO_SELECT: ss << "AUTO"; break;
    }
    
    ss << ", dims=" << dim_m << "x" << dim_n << "x" << dim_k
       << ", priority=" << priority 
       << ", src=0x" << std::hex << src_addr
       << ", dst=0x" << dst_addr << std::dec << "}";
    
    return ss.str();
}
