# CPU Scheduling Simulation Framework

A high-performance C++ framework for simulating and benchmarking various CPU scheduling algorithms. The system simulates online arrival events loaded from a CSV workload, tracks process execution history (turnaround time, wait time, CPU utilization, and throughput), and visualizes the resulting execution schedule as a Gantt chart.

## Framework Architecture

The framework is structured as follows:

1. **Base Scheduler (`Scheduler`)** — Defined in [process.hpp](file:///c:/MY%20Code/CP/mycp/algos/scheduling_algos/process.hpp):
   - Uses the **Curiously Recurring Template Pattern (CRTP)** to hoist common simulation state variables (`cur_`, `has_`, `t_`, `idle_`, `done_`, `gantt_`, `seg_`, and `run_`) and common methods (such as `time()` and `finalize()`) to the base class.
   - Eliminates redundant logic and boilerplates across different algorithms.
2. **Simulation Driver (`run_online`)** — Defined in [process.hpp](file:///c:/MY%20Code/CP/mycp/algos/scheduling_algos/process.hpp):
   - Sorts incoming processes by arrival time and simulates clock ticks.
   - Feeds arrival events dynamically to the scheduler and executes steps until all queues are empty.
   - Uses **move semantics** (`std::move`) to minimize memory allocation and structure copies.
3. **CSV Workload Loader (`load_csv`)** — Defined in [loader.hpp](file:///c:/MY%20Code/CP/mycp/algos/scheduling_algos/loader.hpp):
   - Parses a CSV database of processes (`pid, arrival_time, burst_time, priority`).
   - Built with **exception-safe parsing** to catch conversion issues or blank fields and skip malformed lines gracefully.
4. **Benchmarking Suite (`bench.hpp`)** — Defined in [bench.hpp](file:///c:/MY%20Code/CP/mycp/algos/scheduling_algos/bench.hpp):
   - Runs the simulation over multiple iterations to measure execution wall-clock time (`Wall(us)`), average turnaround time (TAT), average wait time, throughput, and CPU utilization.
   - Renders a comparison table of all scheduler algorithms.

---

## Supported Scheduling Algorithms

All schedulers inherit from `Scheduler<Algorithm>` and implement their core decision-making within the `step()` function:

* **FCFS (First Come First Served)** — Non-preemptive scheduling where processes are executed in the order of their arrival.
* **SJF (Shortest Job First)** — Non-preemptive scheduling that selects the waiting process with the smallest burst time. Optimized with a binary heap (`std::priority_queue`) for $O(\log N)$ selection.
* **SRTF (Shortest Remaining Time First)** — Preemptive SJF where the CPU is reassigned if a process with a shorter remaining time arrives. Optimized with $O(\log N)$ preemption checks using `std::priority_queue`.
* **RR (Round Robin)** — Preemptive scheduling where each process is allocated a fixed time slice (quantum `q`) in a circular queue.
* **Priority (NP & P)** — Selects processes based on their priority value (smaller value denotes higher priority). Supports both Non-Preemptive (NP) and Preemptive (P) modes. Optimized with $O(\log N)$ operations via `std::priority_queue`.
* **MLFQ (Multi-Level Feedback Queue)** — Preemptive scheduler using multiple priority queues with dynamic promotion (aging) to prevent starvation, and demotion based on time-quantum exhaustion.
* **Lottery Scheduling** — Probabilistic scheduler allocating time slices to processes based on tickets (priority values) they hold.
* **Stride Scheduling** — Deterministic proportional-share scheduler that matches lottery scheduling ratios using a "stride" value inversely proportional to priority and a "pass" value. Optimized with $O(\log N)$ selection via `std::priority_queue`.

---

## Getting Started

### Prerequisites
- C++17 compliant compiler (e.g., GCC 9+)
- CMake 3.16+ (optional)

### Compilation
To compile the benchmark executable directly:
```bash
g++ -std=c++17 -O2 main.cpp -o sched
```

### Execution
Run the compiled binary passing the CSV data file and the number of benchmark repetitions as arguments (defaulting to `data.csv` and `5` reps):
```bash
./sched data.csv 5
```
