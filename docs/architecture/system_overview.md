# System Architecture Overview

## 1. Introduction

The Heterogeneous AI Processor is a research platform exploring hardware-software co-design for AI workloads. The system combines specialized compute cores with intelligent scheduling to maximize performance and efficiency.

## 2. System Components

### 2.1 Vector Core
- **Type**: SIMD processor
- **Configuration**: 4-16 lanes (default: 8)
- **Target Workloads**: Element-wise operations, activations, normalization
- **Key Features**:
  - 32 vector registers (256-bit wide)
  - FMA operations
  - Predication support
  - Low-latency execution

### 2.2 Tensor Core
- **Type**: Systolic array
- **Configuration**: 8×8 to 16×16 (default: 8×8)
- **Target Workloads**: Matrix multiplication, convolutions
- **Key Features**:
  - High throughput MAC operations
  - Tiled execution
  - INT8/FP16 support
  - Efficient data reuse

### 2.3 Hardware Scheduler
- **Function**: Dynamic task dispatch and load balancing
- **Features**:
  - Task queue management (32 entries)
  - Core affinity heuristics
  - Priority scheduling
  - Performance monitoring

### 2.4 Memory Subsystem
- **Hierarchy**: L1 scratchpad, L2 shared cache (planned)
- **Current Implementation**: Flat memory model (1MB)
- **Features**:
  - Read/write tracking
  - Bandwidth monitoring
  - DMA support (planned)

### 2.5 Interconnect
- **Type**: Crossbar/bus architecture
- **Bandwidth**: 64 bytes/cycle
- **Ports**: 4 (expandable)
- **Features**:
  - Transaction-based communication
  - Arbitration logic
  - Utilization tracking

## 3. Design Decisions

### 3.1 Why Heterogeneous?
Different AI operations have vastly different compute characteristics:
- **Element-wise ops**: High parallelism, low arithmetic intensity → Vector Core
- **Matrix ops**: High arithmetic intensity, regular access patterns → Tensor Core

### 3.2 Hardware vs. Software Scheduling
**Week 1 Approach**: Hardware scheduler with simple heuristics
- Pros: Low latency, deterministic
- Cons: Less flexible than software

**Future**: Hybrid approach with software hints

### 3.3 Performance Model
Cycle-accurate C++ model allows:
- Early design space exploration
- Performance prediction before RTL
- Software development parallel to hardware

## 4. Performance Targets

### 4.1 Vector Core
- Peak throughput: 8 ops/cycle (8 lanes)
- Typical utilization: 70-80%
- Latency: 4-8 cycles per operation

### 4.2 Tensor Core
- Peak throughput: 128 MAC ops/cycle (8×8 array)
- Efficiency: 80-95% for dense matrices
- Latency: Array_size × K_dimension cycles

### 4.3 System Level
- Target utilization: >80%
- Load balance: <20% variance between cores
- Memory bandwidth: >90% of peak

## 5. Implementation Status

### Week 1 (Current)
- ✅ C++ simulation framework
- ✅ Basic vector core model
- ✅ Basic tensor core model
- ✅ Simple scheduler heuristics
- ✅ Memory and interconnect stubs

### Week 2 (Planned)
- RTL implementation of vector core
- RTL implementation of tensor core
- Cycle-accurate verification
- Performance tuning

### Week 3-4 (Planned)
- Advanced scheduling algorithms
- Software runtime development
- Benchmark suite
- Optimization and analysis

## 6. References

1. NVIDIA Tensor Cores: https://www.nvidia.com/en-us/data-center/tensor-cores/
2. Google TPU Architecture: https://cloud.google.com/tpu/docs/system-architecture
3. ARM Mali GPU Architecture
4. AMD CDNA Architecture

## 7. Future Enhancements

- [ ] Multi-core scaling (4+ cores of each type)
- [ ] Cache hierarchy
- [ ] Power modeling
- [ ] Compression/sparsity support
- [ ] Dynamic voltage/frequency scaling
- [ ] Software compiler integration
