#pragma once
#include "process.hpp"
#include <algorithm>
#include <random>
#include <vector>

class Lottery : public Scheduler<Lottery> {
public:
    static constexpr const char* name = "Lottery";
    explicit Lottery(int q = 4, unsigned seed = 42) : q_(q), rng_(seed) {}

    void submit(Proc p) { p.rem = p.burst; rq_.push_back(std::move(p)); }
    bool busy()  const  { return has_ || !rq_.empty(); }

    void step() {
        if (!has_) {
            if (rq_.empty()) { idle_++; t_++; return; }
            pick_winner();
        }
        cur_.rem--; run_++; t_++;
        if (cur_.rem == 0) {
            cur_.finish = t_;
            gantt_.push_back({cur_.pid, seg_, t_});
            done_.push_back(cur_); has_ = false;
        } else if (run_ >= q_) {
            gantt_.push_back({cur_.pid, seg_, t_});
            rq_.push_back(cur_); has_ = false;
        }
    }

private:
    int q_;
    std::mt19937 rng_;
    std::vector<Proc>       rq_;

    void pick_winner() {
        int total = 0;
        for (auto& p : rq_) total += std::max(1, p.prio);
        std::uniform_int_distribution<int> dist(1, total);
        int ticket = dist(rng_), acc = 0;
        int sel = 0;
        for (int i = 0; i < (int)rq_.size(); i++) {
            acc += std::max(1, rq_[i].prio);
            if (acc >= ticket) { sel = i; break; }
        }
        cur_ = rq_[sel]; rq_.erase(rq_.begin() + sel);
        if (cur_.start == -1) cur_.start = t_;
        seg_ = t_; run_ = 0; has_ = true;
    }
};
