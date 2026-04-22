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

### 1. Deterministic workload

A fixed CPU-bound function using:
- integer arithmetic
- bitwise operations
- deterministic loops

This ensures:
- no I/O effects
- no allocation overhead
- stable execution cost per event

---

### 2. Batch execution model

- N = 1,000,000 events per run
- multiple runs per benchmark cycle
- optional warmup phase

This ensures stable CPU state before measurement.

---

### 3. Measurement methodology

Phase 1 uses **batch timing**:

- measure total execution time of N events
- derive per-event cost: total_time / N
- compute run-level statistics

This avoids distortion from per-event instrumentation overhead.

---

## 📊 Metrics

Each run produces:

- Total execution time (ns)
- Average time per event (ns)
- Throughput (M events/sec)
- Run stability (min / max / avg across runs)

---

## 🧠 Methodology Notes

### Why batch timing?

Per-event timing introduces:
- function call overhead
- timer resolution noise
- cache perturbation

Batch timing ensures measurement overhead does not distort results.

---

### Why multiple runs?

Single runs are unreliable due to:
- CPU frequency scaling
- OS scheduling noise
- background system activity

Multiple runs provide:
- stability validation
- variance estimation

---

## 📈 Example Output
