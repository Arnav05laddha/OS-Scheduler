#pragma once
#include "process.hpp"
#include <deque>
#include <unordered_set>
#include <vector>

class RR : public Scheduler<RR> {
public:
    static constexpr const char* name = "RR";
    explicit RR(int q = 4) : q_(q) {}

    void submit(Proc p) {
        p.rem = p.burst;
        if (!inq_.count(p.pid)) { rq_.push_back(std::move(p)); inq_.insert(p.pid); }
    }
    bool busy()  const { return has_ || !rq_.empty(); }

    void step() {
        if (!has_) {
            if (rq_.empty()) { idle_++; t_++; return; }
            cur_ = rq_.front(); rq_.pop_front(); inq_.erase(cur_.pid);
            if (cur_.start == -1) cur_.start = t_;
            seg_ = t_; run_ = 0; has_ = true;
        }
        cur_.rem--; run_++; t_++;
        if (cur_.rem == 0) {
            cur_.finish = t_;
            gantt_.push_back({cur_.pid, seg_, t_});
            done_.push_back(cur_); has_ = false;
        } else if (run_ >= q_) {
            gantt_.push_back({cur_.pid, seg_, t_});
            rq_.push_back(cur_); inq_.insert(cur_.pid);
            has_ = false;
        }
    }

private:
    int q_;
    std::deque<Proc>            rq_;
    std::unordered_set<int>     inq_;
};
