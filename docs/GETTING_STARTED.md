# Getting Started Guide

## Prerequisites

### Required Software
- **OS**: Linux (Ubuntu 20.04+ recommended) or macOS
- **C++ Compiler**: GCC 7+ or Clang 5+ with C++17 support
- **CMake**: Version 3.10 or later
- **Git**: For version control
- **Verilog Simulator**: Verilator or Icarus Verilog
- **Python**: 3.7+ (for scripts)

### Optional
- **GTKWave**: For waveform viewing
- **Text Editor**: VS Code, Vim, or your preferred editor

## Quick Start (5 minutes)

### 1. Clone Repository
```bash
git clone <repository-url>
cd hetero-ai-processor
```

### 2. Build Everything
```bash
./tools/scripts/build_all.sh
```

This script:
- Builds the C++ simulator
- Tests RTL files
- Runs unit tests

### 3. Run Your First Simulation
```bash
./sim/cpp_model/build/simulator --test
```

You should see output showing:
- Tasks being submitted
- Simulation progress
- Performance statistics

## Detailed Setup

### Step 1: Install Dependencies

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake git \
    verilator iverilog gtkwave python3-pip

pip3 install numpy matplotlib pandas
```

**macOS:**
```bash
brew install cmake verilator icarus-verilog gtkwave
pip3 install numpy matplotlib pandas
```

### Step 2: Build C++ Simulator
```bash
cd sim/cpp_model
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
```

**Build Outputs:**
- `simulator`: Main executable
- `sim_test`: Unit test suite
- `libsim_core.a`: Simulator library

### Step 3: Run Tests
```bash
# Unit tests
./sim_test

# Integration test
./simulator --test

# Custom simulation
./simulator --cycles 5000 --vector-lanes 16 --verbose
```

### Step 4: Verify RTL Toolchain
```bash
cd ../../../rtl/common

# Compile and run testbench
iverilog -o test.vvp test_adder_tb.v test_adder.v
vvp test.vvp

# View waveforms (optional)
gtkwave test_adder.vcd
```

## Project Structure

```
hetero-ai-processor/
├── docs/                    # Documentation
│   ├── architecture/        # System design docs
│   ├── design_decisions/    # Decision log
│   └── GETTING_STARTED.md   # This file
│
├── rtl/                     # RTL source code
│   ├── common/              # Shared modules
│   ├── vector_core/         # Vector processor RTL
│   ├── tensor_core/         # Tensor processor RTL
│   └── scheduler/           # Scheduler RTL
│
├── sim/                     # Simulation models
│   └── cpp_model/           # C++ cycle-accurate simulator
│       ├── include/         # Header files
│       ├── src/             # Implementation
│       └── build/           # Build directory (generated)
│
├── tb/                      # Testbenches
├── sw/                      # Software runtime (future)
├── tools/scripts/           # Build and utility scripts
└── results/                 # Simulation outputs
```

## Common Tasks

### Build from Scratch
```bash
./tools/scripts/clean_all.sh  # Clean everything
./tools/scripts/build_all.sh  # Rebuild
```

### Run Benchmarks
```bash
./tools/scripts/run_benchmarks.sh
```

### Debug Build
```bash
cd sim/cpp_model/build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j$(nproc)
gdb ./simulator
```

### View Simulation Statistics
```bash
# Run with verbose output
./sim/cpp_model/build/simulator --test --verbose > results/sim.log

# Parse statistics
grep "utilization" results/sim.log
```

### Simulator Command-Line Options
```bash
./simulator [options]

Options:
  --test              Run built-in test workload
  --cycles N          Simulate for N cycles (default: 1000)
  --vector-lanes N    Vector core SIMD width (default: 8)
  --tensor-size N     Tensor array dimension (default: 8x8)
  --verbose, -v       Detailed output during simulation
  --help, -h          Show help message

Examples:
  ./simulator --test
  ./simulator --cycles 10000 --vector-lanes 16
  ./simulator --test --tensor-size 16 --verbose
```

## Understanding Output

### Simulation Results Format
```
========================================
--- Simulation Results ---
========================================

[Scheduler Statistics]
  Total cycles:         1000
  Total tasks:          5
  Vector core tasks:    3
  Tensor core tasks:    2
  Vector utilization:   68.50%
  Tensor utilization:   72.30%

[Vector Core Statistics]
  Cycles:               1000
  Tasks completed:      3
  Busy cycles:          685
  Utilization:          68.50%

[Tensor Core Statistics]
  Cycles:               1000
  Tasks completed:      2
  Busy cycles:          723
  MAC operations:       524288
  Utilization:          72.30%
```

### Key metrics:
```bash
  --Utilization: Percentage of time core is actively computing
  --Tasks completed: Number of operations finished
  --MAC operations: Multiply-accumulate count (tensor core)
```

## Troubleshooting
### Build Fails with "C++17 required"
```bash
# Check compiler version
g++ --version  # Need 7.0+
clang++ --version  # Need 5.0+

# Specify compiler explicitly
export CXX=g++-9
cmake ..
```

### "Command not found: cmake"
```bash
# Ubuntu
sudo apt-get install cmake

# macOS
brew install cmake
```

### Simulator Crashes
```bash
# Run with debug build
cd sim/cpp_model/build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j$(nproc)
gdb --args ./simulator --test

# In GDB:
(gdb) run
(gdb) backtrace  # When it crashes
```
### Low Performance/Utilization
```bash
This is expected in Week 1! The scheduler uses simple heuristics.
Improvements coming in Week 2-3:
  --Better load balancing
  --Priority handling
  --Dynamic core selection
```

### "Cannot find header file"
```bash
# Ensure you're in the build directory
cd sim/cpp_model/build
cmake ..
make clean
make -j$(nproc)
```

### RTL Simulation Fails
```bash
# Check Icarus Verilog installation
iverilog -v

# Try with verbose output
iverilog -v -o test.vvp test_adder_tb.v test_adder.v
vvp test.vvp
```
