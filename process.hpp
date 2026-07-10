#pragma once
#include <string>
#include <vector>
#include <algorithm>

struct Proc {
    int pid, arr, burst, prio;
    int rem = 0, start = -1, finish = 0;
    int tat()  const { return finish - arr; }
    int wait() const { return tat() - burst; }
};

struct GanttEntry { int pid, t_start, t_end; };

struct Stats { double avg_tat, avg_wait, throughput, cpu_util; };

struct Result {
    std::vector<Proc>       procs;
    std::vector<GanttEntry> gantt;
    Stats                   stats;
    std::string             algo;
};

inline Stats compute_stats(const std::vector<Proc>& ps, int T, int idle) {
    double tat = 0, wait = 0;
    for (auto& p : ps) { tat += p.tat(); wait += p.wait(); }
    int n = (int)ps.size();
    if (!n || !T) return {};
    return { tat/n, wait/n, (double)n/T, (double)(T-idle)/T };
}

template<typename Derived>
class Scheduler {
public:
    void submit(Proc p) { static_cast<Derived*>(this)->submit(std::move(p)); }
    int  time()  const  { return t_; }
    bool busy()  const  { return static_cast<const Derived*>(this)->busy(); }
    void step()         { static_cast<Derived*>(this)->step(); }
    Result finalize() {
        Result r;
        r.algo = Derived::name;
        r.procs = done_;
        r.gantt = gantt_;
        r.stats = compute_stats(done_, t_, idle_);
        return r;
    }

protected:
    Proc cur_{};
    bool has_ = false;
    int  t_ = 0;
    int  idle_ = 0;
    std::vector<Proc>       done_;
    std::vector<GanttEntry> gantt_;
    int  seg_ = 0;
    int  run_ = 0;
};

// Online simulation driver — feeds events by arrival time, ticks until empty
template<typename Derived>
Result run_online(Scheduler<Derived>& s, std::vector<Proc> evts) {
    std::sort(evts.begin(), evts.end(), [](auto& a, auto& b){ return a.arr < b.arr; });
    int ei = 0, n = (int)evts.size();
    while (ei < n || s.busy()) {
        int t = s.time();
        while (ei < n && evts[ei].arr <= t) s.submit(std::move(evts[ei++]));
        s.step();
    }
    return s.finalize();
}

