#!/bin/bash
# Quick test to verify everything works

set -e

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
cd "$PROJECT_ROOT"

GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo "=========================================="
echo "Running Quick Test"
echo "=========================================="

# Check if built
if [ ! -f "sim/cpp_model/build/simulator" ]; then
    echo "Building project first..."
    ./tools/scripts/build_all.sh
fi

echo -e "\n${YELLOW}Running simulator test...${NC}"
sim/cpp_model/build/simulator --test --cycles 1000

echo ""
echo "=========================================="
echo -e "${GREEN}✓ Quick test passed!${NC}"
echo "=========================================="
