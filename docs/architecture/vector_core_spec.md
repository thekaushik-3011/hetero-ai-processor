# Vector Core Specification

## 1. Overview

The Vector Core is a SIMD processor optimized for parallel element-wise operations common in neural network inference and training.

## 2. Architecture

### 2.1 Register File
- **Vector Registers**: 32 registers (v0-v31)
- **Width**: 256 bits per register
- **Elements**: 8 × 32-bit elements (configurable)
- **Scalar Registers**: 16 × 32-bit (for control/addressing)

### 2.2 Execution Units
- **FMA Units**: 8 (one per lane)
- **Load/Store Units**: 2 (shared across lanes)
- **Special Function Units**: 1 (exp, log, sqrt)

### 2.3 Pipeline
```
[Fetch] → [Decode] → [Execute] → [Writeback]
  1        1          2-4         1
```

Total latency: 5-7 cycles

## 3. Instruction Set

### 3.1 Arithmetic Operations
```
VADD    vd, vs1, vs2      # vd[i] = vs1[i] + vs2[i]
VSUB    vd, vs1, vs2      # vd[i] = vs1[i] - vs2[i]
VMUL    vd, vs1, vs2      # vd[i] = vs1[i] * vs2[i]
VDIV    vd, vs1, vs2      # vd[i] = vs1[i] / vs2[i]
VFMA    vd, vs1, vs2      # vd[i] = vd[i] + vs1[i] * vs2[i]
```

### 3.2 Memory Operations
```
VLD     vd, addr(rs)      # Load vector from memory
VST     vs, addr(rs)      # Store vector to memory
VGATHER vd, base, vindex  # Gather indexed elements
VSCATTER vs, base, vindex # Scatter to indexed locations
```

### 3.3 Special Operations
```
VMAX    vd, vs1, vs2      # Element-wise maximum
VMIN    vd, vs1, vs2      # Element-wise minimum
VABS    vd, vs            # Absolute value
VREDUCE vd, vs, op        # Reduction (sum, max, min)
```

## 4. Performance Model

### 4.1 Throughput
- **Arithmetic**: 1 instruction per cycle (8 ops/cycle with 8 lanes)
- **Load/Store**: 2 vectors per cycle
- **Reductions**: log₂(N) cycles for N elements

### 4.2 Latency
| Operation | Cycles |
|-----------|--------|
| VADD/VSUB | 4      |
| VMUL      | 5      |
| VFMA      | 5      |
| VDIV      | 8      |
| VLD/VST   | 4      |

## 5. Implementation Details

### 5.1 Week 1 (Current)
- Functional model in C++
- Task-based execution
- Cycle estimation
- No instruction-level simulation

### 5.2 Week 2 (Planned)
- RTL implementation in SystemVerilog
- Register file (flip-flops or SRAM)
- Pipeline control logic
- Instruction decoder

### 5.3 Optimization Opportunities
- Vector chaining (back-to-back operations)
- Instruction fusion (combine common patterns)
- Predication for conditional execution
- Multi-bank register file for parallel access

## 6. Usage Example

```cpp
// C++ simulator usage
TaskDescriptor task;
task.type = TaskType::VECTOR_ADD;
task.dim_m = 1024;  // 1024 elements
task.src_addr = 0x1000;
task.dst_addr = 0x2000;

vector_core.submitTask(task);

// Estimated cycles: 1024/8 + overhead = ~133 cycles
```

## 7. Test Cases

### 7.1 Unit Tests
- Single element operations
- Full vector operations
- Boundary conditions
- Pipeline hazards

### 7.2 Integration Tests
- Back-to-back operations
- Memory bandwidth stress
- Mixed workload scenarios

## 8. References

- ARM NEON Architecture
- Intel AVX-512 
- RISC-V Vector Extension v1.0
