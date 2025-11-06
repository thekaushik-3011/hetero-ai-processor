# Week 1 Design Decisions

## Date: November 6, 2024

## 1. Programming Language Choice

### Decision: C++ for Cycle-Accurate Simulator

**Rationale:**
- Performance: 10-100× faster than Python for cycle-level simulation
- Control: Fine-grained control over data structures and execution
- Industry standard: Common in EDA tools (Verilator, SystemC)
- Type safety: Catch errors at compile time

**Alternatives Considered:**
- Python: Rejected due to performance concerns for large simulations
- SystemC: Rejected as overkill for initial modeling
- Rust: Rejected due to team familiarity

## 2. Architecture: Heterogeneous Dual-Core

### Decision: Vector Core + Tensor Core

**Rationale:**
- Workload diversity: AI workloads have different characteristics
- Efficiency: Specialized cores > general purpose for specific tasks
- Manageable scope: 2 cores feasible in 4-week timeline
- Industry relevance: Mirrors real AI accelerators

**Alternatives Considered:**
- Homogeneous multi-core: Less interesting, lower efficiency
- Three+ core types: Too complex for timeframe
- Single programmable core: Misses heterogeneity benefits

## 3. Scheduler: Hardware vs. Software

### Decision: Hardware Scheduler (Week 1 baseline)

**Rationale:**
- Low latency: Dispatch decisions in 1-2 cycles
- Determinism: Predictable behavior for analysis
- Realism: Models actual hardware design
- Evolution path: Can add software layer later

**Week 1 Implementation:**
- Simple heuristic: Task type → core type mapping
- No load balancing (yet)
- No priority handling (yet)
- Queue-based task management

**Future Enhancements:**
- Dynamic load balancing
- Priority scheduling
- Preemption support
- Software hints/overrides

## 4. Memory Model: Flat vs. Hierarchical

### Decision: Flat Memory (Week 1)

**Rationale:**
- Simplicity: Focus on compute cores first
- Flexibility: Easy to refine later
- Sufficient: Adequate for performance trends

**Week 2-3 Plan:**
- Add scratchpad memories
- Model bandwidth constraints
- Add cache hierarchy

## 5. Simulation Granularity

### Decision: Task-Level (Week 1) → Instruction-Level (Week 2+)

**Week 1 Approach:**
- Tasks are atomic units
- Cycle count estimated per task
- No instruction-by-instruction simulation

**Rationale:**
- Faster development: Get end-to-end working quickly
- Sufficient accuracy: ±20% for architecture exploration
- Clear evolution: Natural path to detailed model

## 6. Toolchain: Open Source

### Decision: Verilator + Icarus + GCC/Clang

**Rationale:**
- Accessibility: Anyone can reproduce results
- Cost: Zero licensing fees
- Flexibility: Source available for customization
- Sufficiency: Adequate for educational/research project

**Not chosen:**
- Commercial simulators (ModelSim, VCS): Cost prohibitive
- Proprietary tools: Limits reproducibility

## 7. Project Structure

### Decision: Monorepo with Separate Concerns

**Structure:**
```
hetero-ai-processor/
├── docs/          # Living documentation
├── rtl/           # Hardware description
├── sim/           # Software models
├── sw/            # Runtime software
└── tools/         # Build/test scripts
```

**Rationale:**
- Cohesion: All code in one place
- Traceability: Easy to see hw/sw co-evolution
- Simplicity: Single git repo to clone

## 8. Testing Strategy

### Decision: Layered Testing

**Layers:**
1. Unit tests: Individual components
2. Integration: Multi-component interaction
3. System: Full end-to-end scenarios
4. Benchmarks: Performance characterization

**Week 1 Focus:**
- Unit tests for all major classes
- Basic integration test
- Simple benchmark (mixed workload)

## 9. Documentation Approach

### Decision: Docs-as-Code in Markdown

**Rationale:**
- Version controlled: Track evolution
- Accessible: Readable in GitHub/text editor
- Searchable: Easy to grep/find
- Collaborative: Standard git workflows

**Structure:**
- `docs/architecture/`: System design
- `docs/design_decisions/`: Rationale capture
- `docs/meeting_notes/`: Progress tracking
- Inline code comments: Implementation details

## 10. Performance Targets

### Decision: Modest but Realistic Goals

**Targets:**
- Core utilization: >70%
- Load balance: <30% variance
- Simulation speed: >100K cycles/second

**Rationale:**
- Achievable: Within 4-week scope
- Meaningful: Demonstrates key concepts
- Measurable: Clear success criteria

## 11. Decisions Deferred

The following decisions are explicitly deferred to Week 2+:

1. **Exact instruction set**: Define in detail during RTL design
2. **Cache coherence protocol**: Need multi-core first
3. **Power modeling**: Week 4 if time permits
4. **Compiler integration**: Software runtime first
5. **FPGA prototype**: Out of scope (interesting future work)

## 12. Key Trade-offs

| Decision | Benefit | Cost |
|----------|---------|------|
| C++ over Python | Performance | Development time |
| Task-level sim | Fast development | Initial accuracy |
| Flat memory | Simplicity | Realism |
| Open source tools | Accessibility | Feature limitations |
| Dual-core | Manageable scope | Less impressive demos |

## 13. Success Metrics

How we'll know Week 1 succeeded:
- ✅ All code compiles and runs
- ✅ Unit tests pass
- ✅ Integration test completes
- ✅ Benchmark produces meaningful metrics
- ✅ Documentation captures architecture
- ✅ Git history shows incremental progress

---

**Next Review:** Week 2 retrospective (November 13, 2024)
**Decision Authority:** Project lead (you!)
**Change Process:** Document new decisions in week2_decisions.md
