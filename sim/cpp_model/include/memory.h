//============================================================================
// File: memory.h
// Description: Memory subsystem simulation
//============================================================================

#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include <vector>
#include <unordered_map>
#include <string>

class MemorySubsystem {
public:
    MemorySubsystem(size_t size_bytes = 1024 * 1024);  // 1MB default
    ~MemorySubsystem();
    
    // Memory operations
    void write(uint64_t addr, const void* data, size_t size);
    void read(uint64_t addr, void* data, size_t size);
    
    // Bulk operations
    void writeBlock(uint64_t addr, const std::vector<uint8_t>& data);
    std::vector<uint8_t> readBlock(uint64_t addr, size_t size);
    
    // Memory management
    void clear();
    bool isValidAddress(uint64_t addr, size_t size) const;
    
    // Performance tracking
    void clock();
    uint64_t getCycleCount() const { return cycle_count_; }
    uint64_t getReadCount() const { return read_count_; }
    uint64_t getWriteCount() const { return write_count_; }
    uint64_t getBytesRead() const { return bytes_read_; }
    uint64_t getBytesWritten() const { return bytes_written_; }
    
    // Configuration
    size_t getSize() const { return size_; }
    
private:
    std::vector<uint8_t> memory_;
    uint64_t cycle_count_;
    uint64_t read_count_;
    uint64_t write_count_;
    uint64_t bytes_read_;
    uint64_t bytes_written_;
    size_t size_;
    
    void checkBounds(uint64_t addr, size_t size) const;
};

#endif // MEMORY_H
