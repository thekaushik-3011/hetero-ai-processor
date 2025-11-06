cat > README.md << 'EOF'
# Heterogeneous Multi-Core AI Processor

A hardware/software co-designed AI accelerator featuring heterogeneous compute cores optimized for diverse AI workloads.

## Project Overview

This project implements a dual-core heterogeneous processor combining:
- **Vector Core**: GPU-style SIMD processor for parallel data operations
- **Tensor Core**: Specialized systolic array for matrix/tensor computations
- **Hardware Scheduler**: Dynamic task dispatch and load balancing
- **Software Runtime**: Intelligent workload scheduling and management

## Architecture Highlights

- **Heterogeneous Compute**: Multiple specialized cores for workload diversity
- **Hardware-Software Co-design**: Optimized across hardware and software layers
- **Dynamic Scheduling**: Runtime workload allocation for efficiency
- **Cycle-Accurate Simulation**: C++ simulator for performance analysis

## Project Timeline

- **Week 1 (Nov 6-12)**: Architecture specification & setup
- **Week 2 (Nov 13-19)**: Vector core RTL & initial scheduler
- **Week 3 (Nov 20-26)**: Tensor core RTL & runtime integration
- **Week 4 (Nov 27-30)**: Benchmarking, optimization & documentation

## Directory Structure