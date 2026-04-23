# C Systems Performance Lab

A C-based systems performance exploration focused on understanding CPU execution behavior through controlled benchmarking.

This project is structured as a multi-phase systems lab, where each phase evolves the system from deterministic execution toward real-world event processing.

---

## 🎯 Core Goal

Build a minimal, transparent C benchmarking system to study:

- CPU execution cost
- throughput characteristics
- runtime stability
- measurement methodology in systems programming

The focus is not on features, but on measurable system behavior.

---

## 🧠 Design Philosophy

- Measure first, optimize later
- Minimize instrumentation overhead
- Prefer batch-level measurement over per-event noise
- Treat performance as a system property, not a code property
- Every result must be reproducible

---

# 🧪 Phase 1 — CPU Benchmark Core (Complete)

## 🎯 Objective

Phase 1 establishes a deterministic CPU benchmarking baseline in C.

The goal is to measure raw CPU execution characteristics under controlled conditions.

---

## ⚙️ System Design

### 1. Binary Event Format

Events are represented as a compact structure:
```c
typedef struct {
    uint32_t id;
    uint32_t payload;
} event_t;
```
This ensures a fixed memory footprint and fast serialization/deserialization.

### 2. Replay System

Instead of generating data on the fly, the system replays events from a binary file (`events.bin`).
- Pre-loads all events into memory to avoid I/O interference during measurement.
- Ensures identical input across different benchmarking runs.

### 3. Deterministic Workload

A fixed CPU-bound function processing replayed events:
- integer arithmetic
- bitwise operations
- deterministic loops

This ensures stable execution cost per event.

---

### 4. Batch execution model

- N = 1,000,000 events per run
- multiple runs per benchmark cycle
- warmup phase to prime CPU cache and branch predictor.

---

### 5. Measurement methodology

Phase 1 uses **Cycle-Accurate Timing** via `rdtsc` (with `clock_gettime` fallback):

- Measures total batch time (ns) for throughput.
- Measures per-event latency in CPU cycles.
- Computes statistical distributions (Min, P50, P99, Max).

---

## 📊 Metrics

Each run produces:

- Total execution time (ns)
- Average time per event (ns)
- Throughput (M events/sec)
- Latency Histogram (Cycles): Min, P50 (median), P99 (tail), Max

---

## 🧠 Methodology Notes

### Why rdtsc?

`rdtsc` provides the highest possible resolution on x86 systems, allowing us to measure the cost of processing a single event (often < 100 cycles) with minimal overhead.

### Why p99?

Average latency hides tail performance issues. P99 latency highlights how the system behaves under worst-case scenarios, such as cache misses or OS interrupts.

---

## 📈 Example Output

```text
--- Phase 1 Results ---
Runs: 5 (N=1000000 events each)
Min time: 37092383 ns
Max time: 45869788 ns
Avg time: 38947695.40 ns

Avg per event: 38.95 ns
Throughput: 25.68 M events/sec

--- Latency Histogram (Cycles) ---
Min: 25
P50: 30
P99: 35
Max: 168054
```
