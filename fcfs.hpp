#pragma once
#include "process.hpp"
#include <queue>
#include <vector>

class FCFS : public Scheduler<FCFS> {
public:
    static constexpr const char* name = "FCFS";

    void submit(Proc p) { p.rem = p.burst; rq_.push(std::move(p)); }
    bool busy()  const  { return cur_.rem > 0 || !rq_.empty(); }

    void step() {
        if (cur_.rem == 0) {
            if (rq_.empty()) { idle_++; t_++; return; }
            cur_ = rq_.front(); rq_.pop();
            cur_.start = t_;
        }
        cur_.rem--;
        t_++;
        if (cur_.rem == 0) {
            cur_.finish = t_;
            gantt_.push_back({cur_.pid, cur_.start, t_});
            done_.push_back(cur_);
        }
    }

private:
    std::queue<Proc> rq_;
};
