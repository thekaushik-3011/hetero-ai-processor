#!/bin/bash
# Run all benchmarks and generate reports

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
cd "$PROJECT_ROOT"

SIMULATOR="sim/cpp_model/build/simulator"
RESULTS_DIR="results"

if [ ! -f "$SIMULATOR" ]; then
    echo "Error: Simulator not built. Run build_all.sh first."
    exit 1
fi

mkdir -p "$RESULTS_DIR"

echo "=========================================="
echo "Running Benchmarks"
echo "=========================================="

# Benchmark configurations
CONFIGS=(
    "1000:8:8:Small"
    "5000:8:8:Medium"
    "10000:16:16:Large"
)

for config in "${CONFIGS[@]}"; do
    IFS=':' read -r cycles vlanes tsize name <<< "$config"
    
    echo ""
    echo "Running $name configuration..."
    echo "  Cycles: $cycles, Vector lanes: $vlanes, Tensor size: ${tsize}x${tsize}"
    
    output_file="${RESULTS_DIR}/benchmark_${name}.log"
    
    $SIMULATOR --test --cycles $cycles \
               --vector-lanes $vlanes \
               --tensor-size $tsize > "$output_file"
    
    echo "  ✓ Results saved to $output_file"
done

echo ""
echo "=========================================="
echo "Benchmark suite complete!"
echo "Results in: $RESULTS_DIR"
echo "=========================================="
