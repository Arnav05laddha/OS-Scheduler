#pragma once
#include "process.hpp"
#include <queue>
#include <unordered_map>

class Stride : public Scheduler<Stride> {
public:
    static constexpr const char* name = "Stride";
    explicit Stride(int q = 4) : q_(q), pass_(), rq_(StrideCompare{pass_}) {}

    void submit(Proc p) {
        p.rem = p.burst;
        pass_[p.pid] = 0;
        rq_.push(std::move(p));
    }
    bool busy()  const { return has_ || !rq_.empty(); }

    void step() {
        if (!has_) {
            if (rq_.empty()) { idle_++; t_++; return; }
            pick_next();
        }
        cur_.rem--; run_++; t_++;
        int stride = BIG / std::max(1, cur_.prio);
        pass_[cur_.pid] += stride;

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
    static constexpr int BIG = 10000;
    int q_;
    std::unordered_map<int,int> pass_;
    struct StrideCompare {
        const std::unordered_map<int, int>& pass;
        bool operator()(const Proc& a, const Proc& b) const {
            auto it_a = pass.find(a.pid);
            auto it_b = pass.find(b.pid);
            int pa = (it_a != pass.end()) ? it_a->second : 0;
            int pb = (it_b != pass.end()) ? it_b->second : 0;
            if (pa != pb) return pa > pb;
            return a.pid > b.pid;
        }
    };
    std::priority_queue<Proc, std::vector<Proc>, StrideCompare> rq_;

    void pick_next() {
        cur_ = rq_.top(); rq_.pop();
        if (cur_.start == -1) cur_.start = t_;
        seg_ = t_; run_ = 0; has_ = true;
    }
};
