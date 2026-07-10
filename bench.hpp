#pragma once
#include "process.hpp"
#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

struct BenchResult {
    std::string algo;
    double      wall_us;
    Stats       stats;
};

// Benchmark an online scheduler class constructed fresh each rep
template<typename Sched, typename MakeFn>
BenchResult bench(const std::string& tag, std::vector<Proc>& ps, int reps, MakeFn make) {
    double total = 0;
    Result r;
    for (int i = 0; i < reps; i++) {
        Sched s = make();
        auto  t0 = std::chrono::high_resolution_clock::now();
        r = run_online(s, ps);
        auto  t1 = std::chrono::high_resolution_clock::now();
        total += std::chrono::duration<double, std::micro>(t1 - t0).count();
    }
    return { tag, total / reps, r.stats };
}

inline void print_results(const std::vector<BenchResult>& brs) {
    std::cout << "\n"
              << std::left  << std::setw(14) << "Algorithm"
              << std::right << std::setw(10) << "Wall(us)"
              << std::setw(12) << "Avg TAT"
              << std::setw(12) << "Avg Wait"
              << std::setw(12) << "Throughput"
              << std::setw(10) << "CPU%"
              << "\n" << std::string(70, '-') << "\n";
    for (auto& b : brs) {
        std::cout << std::left  << std::setw(14) << b.algo
                  << std::right << std::fixed << std::setprecision(2)
                  << std::setw(10) << b.wall_us
                  << std::setw(12) << b.stats.avg_tat
                  << std::setw(12) << b.stats.avg_wait
                  << std::setw(12) << b.stats.throughput
                  << std::setw(10) << b.stats.cpu_util * 100.0 << "\n";
    }
    std::cout << "\n";
}

inline void print_gantt(const Result& r, int lim = 25) {
    std::cout << "[" << r.algo << "] Gantt:";
    int cnt = 0;
    for (auto& g : r.gantt) {
        if (cnt++ >= lim) { std::cout << " ..."; break; }
        std::cout << " P" << g.pid << "[" << g.t_start << "-" << g.t_end << "]";
    }
    std::cout << "\n";
}
