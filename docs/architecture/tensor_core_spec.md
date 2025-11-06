# Tensor Core Specification

## 1. Overview

The Tensor Core implements a systolic array architecture for high-throughput matrix multiplication, the fundamental operation in deep learning.

## 2. Systolic Array Architecture

### 2.1 Configuration
- **Default Size**: 8×8 processing elements (64 MACs/cycle)
- **Scalable**: 4×4, 16×16, 32×32 configurations
- **Data Types**: INT8, FP16, FP32 (accumulation)

### 2.2 Processing Element (PE)
Each PE contains:
```
┌──────────────┐
│   Multiplier │ ← A input (horizontal)
│      +       │ ← B input (vertical)
│  Accumulator │ ← Partial sum (diagonal)
└──────────────┘
```

### 2.3 Data Flow
- **Matrix A**: Flows left-to-right
- **Matrix B**: Flows top-to-bottom
- **Partial Sums**: Flow diagonally
- **Stationary**: Weight-stationary or output-stationary modes

## 3. Matrix Multiplication Algorithm

### 3.1 Tiled Execution
For C = A × B where A is M×K, B is K×N:

```
1. Tile matrices into array-sized blocks
2. Stream tiles through systolic array
3. Accumulate partial results
4. Write final output
```

### 3.2 Tile Calculation
- Tile size = Array dimension
- M_tiles = ceil(M / array_size)
- N_tiles = ceil(N / array_size)
- K_tiles = ceil(K / array_size)

### 3.3 Cycle Estimate
```
Cycles = (M_tiles × N_tiles × K_tiles × array_size) + overhead
```

For 128×128×128 on 8×8 array:
```
= (16 × 16 × 16 × 8) + 50
= 32,818 cycles
≈ 260K MACs in 33K cycles = ~8 MACs/cycle efficiency
```

## 4. Supported Operations

### 4.1 Matrix Multiplication
```
C[M,N] = A[M,K] × B[K,N]
```
- Input formats: INT8, FP16
- Accumulation: FP32
- Output: FP16 or FP32

### 4.2 Convolution (2D)
```
Output[H,W,C] = Conv(Input[H,W,C_in], Kernel[K,K,C_in,C])
```
- Implemented as im2col + GEMM
- Efficient for large kernels

### 4.3 Batch Operations
- Multiple matrix multiplications in parallel
- Shared weight matrices across batch

## 5. Performance Model

### 5.1 Peak Performance
- **8×8 Array**: 128 GFLOPS @ 1 GHz (INT8)
- **16×16 Array**: 512 GFLOPS @ 1 GHz
- **Efficiency**: 70-95% depending on tile sizes

### 5.2 Memory Bandwidth Requirements
For peak performance:
```
BW_required = 2 × array_size² × sizeof(element) × freq
            = 2 × 64 × 1 byte × 1 GHz  
            = 128 GB/s (for INT8)
```

## 6. Implementation Details

### 6.1 Week 1 (Current)
- High-level C++ model
- Tile-based execution simulation
- MAC operation counting
- Cycle estimation

### 6.2 Week 3 (Planned RTL)

**PE Module:**
```systemverilog
module processing_element #(
    parameter DATA_WIDTH = 8,
    parameter ACC_WIDTH = 32
)(
    input  clk,
    input  rst_n,
    input  [DATA_WIDTH-1:0] a_in,
    input  [DATA_WIDTH-1:0] b_in,
    input  [ACC_WIDTH-1:0]  sum_in,
    output [DATA_WIDTH-1:0] a_out,
    output [DATA_WIDTH-1:0] b_out,
    output [ACC_WIDTH-1:0]  sum_out
);
    // Implementation
endmodule
```

**Array Controller:**
```systemverilog
module systolic_array #(
    parameter ARRAY_SIZE = 8
)(
    input  clk,
    input  rst_n,
    // Control and data interfaces
);
    // PE instantiation and routing
endmodule
```

## 7. Optimization Techniques

### 7.1 Zero Skipping
- Skip computations when inputs are zero
- Sparse matrix optimization
- Can improve efficiency by 2-4×

### 7.2 Data Reuse
- Weight stationary: Weights stay in PEs
- Input reuse across multiple outputs
- Reduces memory bandwidth

### 7.3 Pipeline Optimization
- Overlap computation and data transfer
- Double buffering for continuous operation
- Hide memory latency

## 8. Usage Example

```cpp
// C++ simulator
TaskDescriptor task;
task.type = TaskType::MATRIX_MUL;
task.dim_m = 128;
task.dim_n = 128;
task.dim_k = 128;
task.src_addr = 0x10000;  // A matrix
task.dst_addr = 0x20000;  // C matrix

tensor_core.submitTask(task);

// Estimated cycles for 8×8 array: ~33K cycles
// Peak MACs: 128³ × 2 = 4.2M operations
// Efficiency: ~8 MACs/cycle (moderate tile size)
```

## 9. Test Strategy

### 9.1 Functional Tests
- Small matrices (exact verification)
- Large matrices (statistical verification)
- Edge cases (1×1, non-square, etc.)

### 9.2 Performance Tests
- Tile size sweep
- Memory bandwidth stress
- Utilization analysis

## 10. References

1. "Why Systolic Architectures?" - H.T. Kung, 1982
2. Google TPU Architecture Paper
3. NVIDIA Tensor Core Documentation
4. Eyeriss: Spatial Architecture for Energy-Efficient Dataflow
