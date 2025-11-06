#!/bin/bash
# Clean all build artifacts

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
cd "$PROJECT_ROOT"

echo "Cleaning build artifacts..."

# Clean C++ build
rm -rf sim/cpp_model/build
echo "  ✓ Cleaned C++ build directory"

# Clean RTL artifacts
find rtl -name "*.vvp" -delete
find rtl -name "*.vcd" -delete
find rtl -name "*.fst" -delete
echo "  ✓ Cleaned RTL artifacts"

# Clean testbench artifacts
find tb -name "*.vvp" -delete
find tb -name "*.vcd" -delete
echo "  ✓ Cleaned testbench artifacts"

# Clean results
rm -rf results/*.csv results/*.png results/*.pdf
echo "  ✓ Cleaned results directory"

echo ""
echo "Clean complete!"
