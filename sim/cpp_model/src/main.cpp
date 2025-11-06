//============================================================================
// File: main.cpp
// Description: Main entry point for the heterogeneous AI processor simulator
//============================================================================

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include "common_types.h"
#include "vector_core.h"
#include "tensor_core.h"
#include "scheduler.h"
#include "memory.h"
#include "interconnect.h"

void printBanner() {
    std::cout << "========================================\n";
    std::cout << "  Heterogeneous AI Processor Simulator\n";
    std::cout << "  Week 1 - Initial Framework\n";
    std::cout << "  Version 1.0\n";
    std::cout << "========================================\n\n";
}

void printHelp(const char* prog_name) {
    std::cout << "Usage: " << prog_name << " [options]\n\n";
    std::cout << "Options:\n";
    std::cout << "  --test              Run basic functionality test\n";
    std::cout << "  --cycles N          Run for N cycles (default: 1000)\n";
    std::cout << "  --vector-lanes N    Set vector core lanes (default: 8)\n";
    std::cout << "  --tensor-size N     Set tensor array size (default: 8)\n";
    std::cout << "  --verbose           Enable verbose output\n";
    std::cout << "  --help              Show this help message\n";
    std::cout << "\nExamples:\n";
    std::cout << "  " << prog_name << " --test\n";
    std::cout << "  " << prog_name << " --cycles 10000 --verbose\n";
}

struct SimConfig {
    int cycles = 1000;
    int vector_lanes = 8;
    int tensor_size = 8;
    bool verbose = false;
    bool run_test = false;
};

SimConfig parseArgs(int argc, char* argv[]) {
    SimConfig config;
    
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "--help" || arg == "-h") {
            printHelp(argv[0]);
            exit(0);
        } else if (arg == "--test") {
            config.run_test = true;
        } else if (arg == "--verbose" || arg == "-v") {
            config.verbose = true;
        } else if (arg == "--cycles" && i + 1 < argc) {
            config.cycles = std::stoi(argv[++i]);
        } else if (arg == "--vector-lanes" && i + 1 < argc) {
            config.vector_lanes = std::stoi(argv[++i]);
        } else if (arg == "--tensor-size" && i + 1 < argc) {
            config.tensor_size = std::stoi(argv[++i]);
        } else {
            std::cerr << "Unknown option: " << arg << "\n";
            printHelp(argv[0]);
            exit(1);
        }
    }
    
    return config;
}

void runBasicTest(const SimConfig& config) {
    std::cout << "Running basic functionality test...\n\n";
    
    // Create system components
    std::cout << "Initializing system components...\n";
    VectorCore vector_core(0, config.vector_lanes);
    TensorCore tensor_core(0, config.tensor_size);
    MemorySubsystem memory(1024 * 1024);  // 1 MB
    Interconnect interconnect(4, 64);     // 4 ports, 64 B/cycle
    
    // Create scheduler
    Scheduler scheduler;
    scheduler.initialize(&vector_core, &tensor_core);
    
    std::cout << "\n--- Creating Test Workload ---\n";
    
    // Create diverse test tasks
    std::vector<TaskDescriptor> tasks;
    
    // Task 1: Vector addition
    TaskDescriptor task1;
    task1.type = TaskType::VECTOR_ADD;
    task1.dim_m = 1024;
    task1.priority = 1;
    task1.src_addr = 0x0000;
    task1.dst_addr = 0x1000;
    tasks.push_back(task1);
    
    // Task 2: Matrix multiplication (small)
    TaskDescriptor task2;
    task2.type = TaskType::MATRIX_MUL;
    task2.dim_m = 64;
    task2.dim_n = 64;
    task2.dim_k = 64;
    task2.priority = 2;
    task2.src_addr = 0x2000;
    task2.dst_addr = 0x4000;
    tasks.push_back(task2);
    
    // Task 3: Vector FMA
    TaskDescriptor task3;
    task3.type = TaskType::VECTOR_FMA;
    task3.dim_m = 2048;
    task3.priority = 1;
    task3.src_addr = 0x6000;
    task3.dst_addr = 0x8000;
    tasks.push_back(task3);
    
    // Task 4: Matrix multiplication (larger)
    TaskDescriptor task4;
    task4.type = TaskType::MATRIX_MUL;
    task4.dim_m = 128;
    task4.dim_n = 128;
    task4.dim_k = 128;
    task4.priority = 2;
    task4.src_addr = 0xA000;
    task4.dst_addr = 0xC000;
    tasks.push_back(task4);
    
    // Task 5: Vector multiplication
    TaskDescriptor task5;
    task5.type = TaskType::VECTOR_MUL;
    task5.dim_m = 512;
    task5.priority = 1;
    task5.src_addr = 0xE000;
    task5.dst_addr = 0xF000;
    tasks.push_back(task5);
    
    // Submit tasks
    std::cout << "Submitting " << tasks.size() << " tasks:\n";
    for (size_t i = 0; i < tasks.size(); i++) {
        if (scheduler.submitTask(tasks[i])) {
            std::cout << "  Task " << (i+1) << ": " << tasks[i].toString() << "\n";
        } else {
            std::cout << "  Task " << (i+1) << ": FAILED to submit\n";
        }
    }
    
    // Run simulation
    std::cout << "\n--- Running Simulation ---\n";
    std::cout << "Simulating " << config.cycles << " cycles...\n";
    
    int progress_step = config.cycles / 10;
    for (int cycle = 0; cycle < config.cycles; cycle++) {
        scheduler.clock();
        vector_core.clock();
        tensor_core.clock();
        memory.clock();
        interconnect.clock();
        
        if (config.verbose && (cycle % 100 == 0)) {
            std::cout << "  Cycle " << cycle << " - Queue depth: " 
                      << scheduler.getQueueDepth() << "\n";
        } else if (!config.verbose && progress_step > 0 && (cycle % progress_step == 0)) {
            std::cout << "  Progress: " << (cycle * 100 / config.cycles) << "%\r" << std::flush;
        }
    }
    std::cout << "  Progress: 100%    \n";
    
    // Print results
    std::cout << "\n========================================\n";
    std::cout << "--- Simulation Results ---\n";
    std::cout << "========================================\n";
    
    PerfStats stats = scheduler.getStats();
    
    std::cout << "\n[Scheduler Statistics]\n";
    std::cout << "  Total cycles:         " << stats.total_cycles << "\n";
    std::cout << "  Total tasks:          " << stats.total_tasks << "\n";
    std::cout << "  Vector core tasks:    " << stats.vector_core_tasks << "\n";
    std::cout << "  Tensor core tasks:    " << stats.tensor_core_tasks << "\n";
    std::cout << "  Vector utilization:   " << std::fixed << std::setprecision(2) 
              << stats.vector_utilization() * 100 << "%\n";
    std::cout << "  Tensor utilization:   " << std::fixed << std::setprecision(2)
              << stats.tensor_utilization() * 100 << "%\n";
    
    std::cout << "\n[Vector Core Statistics]\n";
    std::cout << "  Cycles:               " << vector_core.getCycleCount() << "\n";
    std::cout << "  Tasks completed:      " << vector_core.getTaskCount() << "\n";
    std::cout << "  Busy cycles:          " << vector_core.getBusyCycles() << "\n";
    std::cout << "  Utilization:          " << std::fixed << std::setprecision(2)
              << (vector_core.getCycleCount() > 0 ? 
                  (100.0 * vector_core.getBusyCycles() / vector_core.getCycleCount()) : 0.0) 
              << "%\n";
    
    std::cout << "\n[Tensor Core Statistics]\n";
    std::cout << "  Cycles:               " << tensor_core.getCycleCount() << "\n";
    std::cout << "  Tasks completed:      " << tensor_core.getTaskCount() << "\n";
    std::cout << "  Busy cycles:          " << tensor_core.getBusyCycles() << "\n";
    std::cout << "  MAC operations:       " << tensor_core.getMACOperations() << "\n";
    std::cout << "  Utilization:          " << std::fixed << std::setprecision(2)
              << (tensor_core.getCycleCount() > 0 ? 
                  (100.0 * tensor_core.getBusyCycles() / tensor_core.getCycleCount()) : 0.0) 
              << "%\n";
    
    std::cout << "\n[Memory Statistics]\n";
    std::cout << "  Cycles:               " << memory.getCycleCount() << "\n";
    std::cout << "  Read operations:      " << memory.getReadCount() << "\n";
    std::cout << "  Write operations:     " << memory.getWriteCount() << "\n";
    std::cout << "  Bytes read:           " << memory.getBytesRead() << "\n";
    std::cout << "  Bytes written:        " << memory.getBytesWritten() << "\n";
    
    std::cout << "\n[Interconnect Statistics]\n";
    std::cout << "  Cycles:               " << interconnect.getCycleCount() << "\n";
    std::cout << "  Transactions:         " << interconnect.getTransactionCount() << "\n";
    std::cout << "  Bytes transferred:    " << interconnect.getTotalBytesTransferred() << "\n";
    std::cout << "  Utilization:          " << std::fixed << std::setprecision(2)
              << interconnect.getUtilization() * 100 << "%\n";
    
    std::cout << "\n========================================\n";
    std::cout << "✓ Test completed successfully!\n";
    std::cout << "========================================\n";
}

int main(int argc, char* argv[]) {
    printBanner();
    
    SimConfig config = parseArgs(argc, argv);
    
    if (config.run_test || argc == 1) {
        runBasicTest(config);
    } else {
        printHelp(argv[0]);
    }
    
    return 0;
}
