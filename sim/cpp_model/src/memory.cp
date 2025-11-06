#include "memory.h"
#include <cstring>
#include <iostream>
#include <stdexcept>

MemorySubsystem::MemorySubsystem(size_t size_bytes)
    : memory_(size_bytes, 0), cycle_count_(0), read_count_(0), 
      write_count_(0), bytes_read_(0), bytes_written_(0), size_(size_bytes) {
    std::cout << "[Memory] Initialized " << size_bytes / 1024 << " KB" << std::endl;
}

MemorySubsystem::~MemorySubsystem() {
    std::cout << "[Memory] Final stats - Reads: " << read_count_ 
              << ", Writes: " << write_count_ << std::endl;
}

void MemorySubsystem::write(uint64_t addr, const void* data, size_t size) {
    checkBounds(addr, size);
    std::memcpy(&memory_[addr], data, size);
    write_count_++;
    bytes_written_ += size;
}

void MemorySubsystem::read(uint64_t addr, void* data, size_t size) {
    checkBounds(addr, size);
    std::memcpy(data, &memory_[addr], size);
    read_count_++;
    bytes_read_ += size;
}

void MemorySubsystem::writeBlock(uint64_t addr, const std::vector<uint8_t>& data) {
    write(addr, data.data(), data.size());
}

std::vector<uint8_t> MemorySubsystem::readBlock(uint64_t addr, size_t size) {
    checkBounds(addr, size);
    std::vector<uint8_t> result(size);
    read(addr, result.data(), size);
    return result;
}

void MemorySubsystem::clear() {
    std::fill(memory_.begin(), memory_.end(), 0);
}

bool MemorySubsystem::isValidAddress(uint64_t addr, size_t size) const {
    return (addr + size <= size_);
}

void MemorySubsystem::clock() {
    cycle_count_++;
}

void MemorySubsystem::checkBounds(uint64_t addr, size_t size) const {
    if (!isValidAddress(addr, size)) {
        std::cerr << "[Memory] ERROR: Access out of bounds - Address: 0x" 
                  << std::hex << addr << ", Size: " << std::dec << size 
                  << ", Memory size: " << size_ << std::endl;
        throw std::out_of_range("Memory access out of bounds");
    }
}
