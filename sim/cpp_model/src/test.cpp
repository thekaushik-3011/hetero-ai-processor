//============================================================================
// File: test.cpp
// Description: Unit tests for simulator components
//============================================================================

#include <iostream>
#include <cassert>
#include <vector>
#include "common_types.h"
#include "vector_core.h"
#include "tensor_core.h"
#include "scheduler.h"
#include "memory.h"
#include "interconnect.h"

int tests_passed = 0;
int tests_failed = 0;

#define TEST_ASSERT(condition, message) \
    if (!(condition)) { \
        std::cerr << "  ✗ FAILED: " << message << "\n"; \
        tests_failed++; \
        return; \
    }

void testVectorCore() {
    std::cout << "\n[Test] VectorCore functionality...\n";
    VectorCore core(0, 8);
    
    TEST_ASSERT(core.isIdle() == true, "Core should start idle");
    TEST_ASSERT(core.getCycleCount() == 0, "Initial cycle count should be 0");
    TEST_ASSERT(core.getNumLanes() == 8, "Core should have 8 lanes");
    
    TaskDescriptor task;
    task.type = TaskType::VECTOR_ADD;
    task.dim_m = 256;
    
    TEST_ASSERT(core.submitTask(task) == true, "Should accept task");
    
    // Run for 200 cycles
    for (int i = 0; i < 200; i++) {
        core.clock();
    }
    
    TEST_ASSERT(core.getCycleCount() == 200, "Should have run 200 cycles");
    TEST_ASSERT(core.getTaskCount() >= 1, "Should have completed at least 1 task");
    
    std::cout << "  ✓ VectorCore tests passed\n";
    tests_passed++;
}

void testTensorCore() {
    std::cout << "\n[Test] TensorCore functionality...\n";
    TensorCore core(0, 8);
    
    TEST_ASSERT(core.isIdle() == true, "Core should start idle");
    TEST_ASSERT(core.getCycleCount() == 0, "Initial cycle count should be 0");
    TEST_ASSERT(core.getArraySize() == 8, "Array should be 8x8");
    
    TaskDescriptor task;
    task.type = TaskType::MATRIX_MUL;
    task.dim_m = 32;
    task.dim_n = 32;
    task.dim_k = 32;
    
    TEST_ASSERT(core.submitTask(task) == true, "Should accept task");
    
    // Run for 300 cycles
    for (int i = 0; i < 300; i++) {
        core.clock();
    }
    
    TEST_ASSERT(core.getCycleCount() == 300, "Should have run 300 cycles");
    TEST_ASSERT(core.getTaskCount() >= 1, "Should have completed at least 1 task");
    TEST_ASSERT(core.getMACOperations() > 0, "Should have performed MAC operations");
    
    std::cout << "  ✓ TensorCore tests passed\n";
    tests_passed++;
}

void testScheduler() {
    std::cout << "\n[Test] Scheduler functionality...\n";
    
    VectorCore vcore(0, 8);
    TensorCore tcore(0, 8);
    Scheduler scheduler;
    
    scheduler.initialize(&vcore, &tcore);
    
    TaskDescriptor task1, task2;
    task1.type = TaskType::VECTOR_ADD;
    task1.dim_m = 512;
    task2.type = TaskType::MATRIX_MUL;
    task2.dim_m = 64;
    task2.dim_n = 64;
    task2.dim_k = 64;
    
    TEST_ASSERT(scheduler.submitTask(task1) == true, "Should accept vector task");
    TEST_ASSERT(scheduler.submitTask(task2) == true, "Should accept tensor task");
    
    // Run simulation
    for (int i = 0; i < 500; i++) {
        scheduler.clock();
        vcore.clock();
        tcore.clock();
    }
    
    PerfStats stats = scheduler.getStats();
    TEST_ASSERT(stats.total_cycles == 500, "Should have run 500 cycles");
    TEST_ASSERT(stats.total_tasks == 2, "Should have submitted 2 tasks");
    TEST_ASSERT(stats.vector_core_tasks > 0, "Should have dispatched vector tasks");
    TEST_ASSERT(stats.tensor_core_tasks > 0, "Should have dispatched tensor tasks");
    
    std::cout << "  ✓ Scheduler tests passed\n";
    tests_passed++;
}

void testMemorySubsystem() {
    std::cout << "\n[Test] Memory subsystem...\n";
    
    MemorySubsystem mem(1024);  // 1KB
    
    TEST_ASSERT(mem.getSize() == 1024, "Memory size should be 1KB");
    TEST_ASSERT(mem.getCycleCount() == 0, "Initial cycle count should be 0");
    
    // Test write and read
    uint32_t test_data = 0xDEADBEEF;
    mem.write(0, &test_data, sizeof(test_data));
    
    uint32_t read_data = 0;
    mem.read(0, &read_data, sizeof(read_data));
    
    TEST_ASSERT(read_data == test_data, "Read data should match written data");
    TEST_ASSERT(mem.getWriteCount() == 1, "Should have 1 write operation");
    TEST_ASSERT(mem.getReadCount() == 1, "Should have 1 read operation");
    
    // Test block operations
    std::vector<uint8_t> block_data = {1, 2, 3, 4, 5};
    mem.writeBlock(100, block_data);
    std::vector<uint8_t> read_block = mem.readBlock(100, 5);
    
    TEST_ASSERT(read_block == block_data, "Block read should match block write");
    
    // Test bounds checking
    TEST_ASSERT(mem.isValidAddress(0, 1024) == true, "Valid address should pass");
    TEST_ASSERT(mem.isValidAddress(1024, 1) == false, "Out of bounds should fail");
    
    std::cout << "  ✓ Memory subsystem tests passed\n";
    tests_passed++;
}

void testInterconnect() {
    std::cout << "\n[Test] Interconnect...\n";
    
    Interconnect ic(4, 64);  // 4 ports, 64 B/cycle
    
    TEST_ASSERT(ic.getNumPorts() == 4, "Should have 4 ports");
    TEST_ASSERT(ic.getBandwidth() == 64, "Should have 64 B/cycle bandwidth");
    
    // Create test transaction
    Transaction trans;
    trans.type = TransactionType::READ_REQUEST;
    trans.source_id = 0;
    trans.dest_id = 1;
    trans.address = 0x1000;
    trans.size = 128;
    
    TEST_ASSERT(ic.submitTransaction(trans) == true, "Should accept transaction");
    
    // Run for some cycles
    for (int i = 0; i < 100; i++) {
        ic.clock();
    }
    
    TEST_ASSERT(ic.getCycleCount() == 100, "Should have run 100 cycles");
    TEST_ASSERT(ic.getTransactionCount() > 0, "Should have completed transactions");
    
    std::cout << "  ✓ Interconnect tests passed\n";
    tests_passed++;
}

void testTaskDescriptor() {
    std::cout << "\n[Test] TaskDescriptor...\n";
    
    TaskDescriptor task;
    task.type = TaskType::MATRIX_MUL;
    task.preferred_core = CoreType::TENSOR_CORE;
    task.dim_m = 128;
    task.dim_n = 128;
    task.dim_k = 128;
    task.priority = 5;
    task.src_addr = 0x10000;
    task.dst_addr = 0x20000;
    
    std::string str = task.toString();
    TEST_ASSERT(str.find("MATRIX_MUL") != std::string::npos, "Should contain task type");
    TEST_ASSERT(str.find("TENSOR") != std::string::npos, "Should contain core type");
    TEST_ASSERT(str.find("128") != std::string::npos, "Should contain dimensions");
    
    std::cout << "  Task string: " << str << "\n";
    std::cout << "  ✓ TaskDescriptor tests passed\n";
    tests_passed++;
}

void testIntegration() {
    std::cout << "\n[Test] Integration test - Full system...\n";
    
    // Create all components
    VectorCore vcore(0, 8);
    TensorCore tcore(0, 8);
    MemorySubsystem mem(64 * 1024);  // 64KB
    Interconnect ic(4, 64);
    Scheduler scheduler;
    
    scheduler.initialize(&vcore, &tcore);
    
    // Create mixed workload
    std::vector<TaskDescriptor> tasks;
    
    for (int i = 0; i < 10; i++) {
        TaskDescriptor task;
        if (i % 2 == 0) {
            task.type = TaskType::VECTOR_ADD;
            task.dim_m = 256 * (i + 1);
        } else {
            task.type = TaskType::MATRIX_MUL;
            task.dim_m = 32 * (i + 1);
            task.dim_n = 32 * (i + 1);
            task.dim_k = 32;
        }
        task.priority = i % 3;
        tasks.push_back(task);
    }
    
    // Submit all tasks
    int submitted = 0;
    for (const auto& task : tasks) {
        if (scheduler.submitTask(task)) {
            submitted++;
        }
    }
    
    TEST_ASSERT(submitted == tasks.size(), "Should submit all tasks");
    
    // Run full system simulation
    for (int i = 0; i < 2000; i++) {
        scheduler.clock();
        vcore.clock();
        tcore.clock();
        mem.clock();
        ic.clock();
    }
    
    PerfStats stats = scheduler.getStats();
    TEST_ASSERT(stats.total_cycles == 2000, "Should run 2000 cycles");
    TEST_ASSERT(vcore.getTaskCount() > 0, "Vector core should complete tasks");
    TEST_ASSERT(tcore.getTaskCount() > 0, "Tensor core should complete tasks");
    
    double total_util = stats.vector_utilization() + stats.tensor_utilization();
    TEST_ASSERT(total_util > 0.0, "System should have non-zero utilization");
    
    std::cout << "  System utilization: " << (total_util / 2.0 * 100) << "%\n";
    std::cout << "  ✓ Integration test passed\n";
    tests_passed++;
}

void printTestSummary() {
    std::cout << "\n========================================\n";
    std::cout << "Test Summary\n";
    std::cout << "========================================\n";
    std::cout << "Tests passed: " << tests_passed << "\n";
    std::cout << "Tests failed: " << tests_failed << "\n";
    std::cout << "Total tests:  " << (tests_passed + tests_failed) << "\n";
    
    if (tests_failed == 0) {
        std::cout << "\n✓ All tests passed successfully!\n";
    } else {
        std::cout << "\n✗ Some tests failed!\n";
    }
    std::cout << "========================================\n";
}

int main() {
    std::cout << "========================================\n";
    std::cout << "  Running Unit Tests\n";
    std::cout << "========================================\n";
    
    testTaskDescriptor();
    testVectorCore();
    testTensorCore();
    testScheduler();
    testMemorySubsystem();
    testInterconnect();
    testIntegration();
    
    printTestSummary();
    
    return (tests_failed == 0) ? 0 : 1;
}
