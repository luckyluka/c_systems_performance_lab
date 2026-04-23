# C Systems Performance Lab

A 1-year engineering project focused on building and evolving a high-performance event processing system in C.

This project is designed to explore how systems behave under increasing real-world constraints: deterministic execution → streaming input → concurrency → production-style benchmarking.

---

## 🎯 Core Goal

Build a C-based event processing system that evolves through multiple stages:

1. Deterministic CPU-bound processing
2. Real-time streaming input
3. Concurrent multi-threaded execution
4. Production-style benchmarking and tooling

The focus is not on features, but on:

- latency (p50 / p99 / max)
- throughput (events/sec)
- jitter behavior
- system-level performance characteristics

---

## 🧠 Design Philosophy

- Prefer measurement over assumption
- Prefer simplicity over frameworks
- Prefer system behavior understanding over abstraction
- Every change must produce measurable impact

---

## 🧭 1-Year Roadmap

### 🥇 Phase 1 — Deterministic Core (Months 1–3)
**Status:** COMPLETE

Build a CPU-bound event processing engine.

Focus:
- binary event format (COMPLETED)
- deterministic execution loop (COMPLETED)
- replay system (file-based input) (COMPLETED)
- cycle-level timing (rdtsc or equivalent) (COMPLETED)
- latency histogram (p50/p99/max) (COMPLETED)

Outcome:
> Understand and measure raw CPU performance behavior

---

### 🥈 Phase 2 — Streaming Input Systems (Months 4–6)
**Status:** LOCKED

Extend system with real-time input sources.

Focus:
- pipes (stdin streaming)
- FIFO (named pipes)
- buffered input behavior
- backpressure and burst handling
- jitter analysis under I/O load

Outcome:
> Understand OS-level input constraints and real-time behavior

---

### 🥉 Phase 3 — Concurrency & Scaling (Months 7–9)
**Status:** LOCKED

Introduce multi-threaded processing.

Focus:
- producer / consumer architecture
- ring buffers
- lock contention analysis
- CPU scaling behavior
- throughput vs latency tradeoffs

Outcome:
> Understand how systems scale and where bottlenecks appear

---

### 🏁 Phase 4 — System Maturity (Months 10–12)
**Status:** LOCKED

Turn system into a structured performance tool.

Focus:
- multiple execution modes (replay / streaming / benchmark)
- structured CLI interface
- regression benchmarking
- reproducible performance runs
- clean system architecture

Outcome:
> Build a production-style performance measurement tool

---

## 📊 Metrics Tracked

Each stage measures:

- Throughput (events/sec)
- Latency distribution (p50 / p99 / max)
- Jitter under load
- CPU cost per event
- Scaling efficiency (multi-thread vs single-thread)

---

## 🧪 Current Status

```text
Phase 1: Deterministic Core
- Event loop implementation: COMPLETE
- Replay system: COMPLETE
- Latency measurement: COMPLETE
```
