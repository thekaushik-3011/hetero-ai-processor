#!/bin/bash
# Complete build script for the entire project

set -e

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
cd "$PROJECT_ROOT"

echo "=========================================="
echo "Building Heterogeneous AI Processor"
echo "=========================================="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

# Build C++ simulator
echo -e "\n${YELLOW}[1/3] Building C++ Simulator...${NC}"
cd sim/cpp_model
mkdir -p build
cd build

if cmake -DCMAKE_BUILD_TYPE=Release .. && make -j$(nproc); then
    echo -e "${GREEN}✓ C++ Simulator built successfully${NC}"
else
    echo -e "${RED}✗ C++ Simulator build failed${NC}"
    exit 1
fi

cd "$PROJECT_ROOT"

# Test RTL
echo -e "\n${YELLOW}[2/3] Testing RTL...${NC}"
cd rtl/common

if iverilog -o test_adder.vvp test_adder_tb.v test_adder.v && vvp test_adder.vvp > /dev/null 2>&1; then
    echo -e "${GREEN}✓ RTL tests passed${NC}"
    rm -f test_adder.vvp test_adder.vcd
else
    echo -e "${RED}✗ RTL tests failed${NC}"
    exit 1
fi

cd "$PROJECT_ROOT"

# Run unit tests
echo -e "\n${YELLOW}[3/3] Running Unit Tests...${NC}"
if sim/cpp_model/build/sim_test > /dev/null 2>&1; then
    echo -e "${GREEN}✓ Unit tests passed${NC}"
else
    echo -e "${RED}✗ Unit tests failed${NC}"
    exit 1
fi

echo ""
echo "=========================================="
echo -e "${GREEN}✓ Build Complete!${NC}"
echo "=========================================="
echo ""
echo "Executables:"
echo "  - sim/cpp_model/build/simulator"
echo "  - sim/cpp_model/build/sim_test"
echo ""
echo "Run: ./sim/cpp_model/build/simulator --test"
