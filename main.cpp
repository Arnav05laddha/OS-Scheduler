#include "bench.hpp"
#include "fcfs.hpp"
#include "sjf.hpp"
#include "srtf.hpp"
#include "rr.hpp"
#include "priority.hpp"
#include "mlfq.hpp"
#include "lottery.hpp"
#include "stride.hpp"
#include "loader.hpp"
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
    std::string path = "data.csv";
    int reps = 5;
    if (argc > 1) path = argv[1];
    if (argc > 2) reps = std::stoi(argv[2]);

    std::vector<Proc> ps;
    try { ps = load_csv(path); }
    catch (std::exception& e) { std::cerr << e.what() << "\n"; return 1; }

    std::cout << "Loaded " << ps.size() << " processes | reps=" << reps << "\n";

    std::vector<BenchResult> res;

    res.push_back(bench<FCFS>      ("FCFS",        ps, reps, []{ return FCFS{}; }));
    res.push_back(bench<SJF>       ("SJF",         ps, reps, []{ return SJF{}; }));
    res.push_back(bench<SRTF>      ("SRTF",        ps, reps, []{ return SRTF{}; }));
    res.push_back(bench<RR>        ("RR(q=4)",     ps, reps, []{ return RR{4}; }));
    res.push_back(bench<Priority>  ("Priority-NP", ps, reps, []{ return Priority{false}; }));
    res.push_back(bench<Priority>  ("Priority-P",  ps, reps, []{ return Priority{true}; }));
    res.push_back(bench<MLFQ>      ("MLFQ",        ps, reps, []{ return MLFQ{3, 4, 20}; }));
    res.push_back(bench<Lottery>   ("Lottery",     ps, reps, []{ return Lottery{4, 42}; }));
    res.push_back(bench<Stride>    ("Stride",      ps, reps, []{ return Stride{4}; }));

    print_results(res);

    // sample gantt from FCFS
    FCFS f;
    auto r = run_online(f, ps);
    print_gantt(r);

    return 0;
}
