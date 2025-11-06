//============================================================================
// File: interconnect.h
// Description: Interconnect/bus simulation for core communication
//============================================================================

#ifndef INTERCONNECT_H
#define INTERCONNECT_H

#include <cstdint>
#include <queue>
#include <vector>

// Transaction types
enum class TransactionType {
    READ_REQUEST,
    WRITE_REQUEST,
    READ_RESPONSE,
    WRITE_RESPONSE
};

// Transaction descriptor
struct Transaction {
    TransactionType type;
    int source_id;
    int dest_id;
    uint64_t address;
    size_t size;
    uint64_t timestamp;
    
    Transaction() : type(TransactionType::READ_REQUEST), source_id(0), 
                    dest_id(0), address(0), size(0), timestamp(0) {}
};

class Interconnect {
public:
    Interconnect(int num_ports = 4, int bandwidth_bytes_per_cycle = 64);
    ~Interconnect();
    
    // Transaction interface
    bool submitTransaction(const Transaction& trans);
    bool hasCompletedTransaction(int port_id) const;
    Transaction getCompletedTransaction(int port_id);
    
    // Simulation
    void clock();
    void reset();
    
    // Performance counters
    uint64_t getCycleCount() const { return cycle_count_; }
    uint64_t getTransactionCount() const { return transaction_count_; }
    uint64_t getTotalBytesTransferred() const { return total_bytes_; }
    double getUtilization() const;
    
    // Configuration
    int getNumPorts() const { return num_ports_; }
    int getBandwidth() const { return bandwidth_; }
    
private:
    int num_ports_;
    int bandwidth_;  // Bytes per cycle
    uint64_t cycle_count_;
    uint64_t transaction_count_;
    uint64_t total_bytes_;
    uint64_t busy_cycles_;
    
    std::queue<Transaction> pending_queue_;
    std::vector<std::queue<Transaction>> completion_queues_;
    
    Transaction current_transaction_;
    int cycles_remaining_;
    bool processing_;
    
    static constexpr int MAX_QUEUE_DEPTH = 32;
    
    void processTransaction();
    int calculateTransactionCycles(const Transaction& trans) const;
};

#endif // INTERCONNECT_H
EOF

cat > src/interconnect.cpp << 'EOF'
#include "interconnect.h"
#include <iostream>
#include <algorithm>

Interconnect::Interconnect(int num_ports, int bandwidth_bytes_per_cycle)
    : num_ports_(num_ports), bandwidth_(bandwidth_bytes_per_cycle),
      cycle_count_(0), transaction_count_(0), total_bytes_(0), 
      busy_cycles_(0), cycles_remaining_(0), processing_(false) {
    
    completion_queues_.resize(num_ports);
    std::cout << "[Interconnect] Initialized with " << num_ports_ 
              << " ports, " << bandwidth_ << " B/cycle bandwidth" << std::endl;
}

Interconnect::~Interconnect() {
    std::cout << "[Interconnect] Total transactions: " << transaction_count_
              << ", Utilization: " << getUtilization() * 100 << "%" << std::endl;
}

bool Interconnect::submitTransaction(const Transaction& trans) {
    if (pending_queue_.size() >= MAX_QUEUE_DEPTH) {
        return false;
    }
    pending_queue_.push(trans);
    return true;
}

bool Interconnect::hasCompletedTransaction(int port_id) const {
    if (port_id < 0 || port_id >= num_ports_) {
        return false;
    }
    return !completion_queues_[port_id].empty();
}

Transaction Interconnect::getCompletedTransaction(int port_id) {
    if (port_id < 0 || port_id >= num_ports_ || completion_queues_[port_id].empty()) {
        return Transaction();
    }
    Transaction trans = completion_queues_[port_id].front();
    completion_queues_[port_id].pop();
    return trans;
}

void Interconnect::clock() {
    cycle_count_++;
    
    if (processing_) {
        busy_cycles_++;
        cycles_remaining_--;
        
        if (cycles_remaining_ <= 0) {
            // Transaction complete
            completion_queues_[current_transaction_.dest_id].push(current_transaction_);
            processing_ = false;
            transaction_count_++;
        }
    }
    
    // Start new transaction if available
    if (!processing_ && !pending_queue_.empty()) {
        current_transaction_ = pending_queue_.front();
        pending_queue_.pop();
        
        cycles_remaining_ = calculateTransactionCycles(current_transaction_);
        processing_ = true;
        total_bytes_ += current_transaction_.size;
    }
}

void Interconnect::reset() {
    while (!pending_queue_.empty()) pending_queue_.pop();
    for (auto& q : completion_queues_) {
        while (!q.empty()) q.pop();
    }
    cycle_count_ = 0;
    transaction_count_ = 0;
    total_bytes_ = 0;
    busy_cycles_ = 0;
    processing_ = false;
}

double Interconnect::getUtilization() const {
    return cycle_count_ > 0 ? static_cast<double>(busy_cycles_) / cycle_count_ : 0.0;
}

int Interconnect::calculateTransactionCycles(const Transaction& trans) const {
    // Calculate cycles based on size and bandwidth
    int cycles = (trans.size + bandwidth_ - 1) / bandwidth_;
    return std::max(1, cycles);  // At least 1 cycle
}

void Interconnect::processTransaction() {
    // This method could be extended for more complex arbitration logic
}
