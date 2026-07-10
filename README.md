# CPU Scheduling Simulation Framework

A high-performance C++ framework for simulating and benchmarking various CPU scheduling algorithms. The system simulates online arrival events loaded from a CSV workload, tracks process execution history (turnaround time, wait time, CPU utilization, and throughput), and visualizes the resulting execution schedule as a Gantt chart.


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
