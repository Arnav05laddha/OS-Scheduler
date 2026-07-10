#pragma once
#include "process.hpp"
#include <queue>

class SJF : public Scheduler<SJF> {
public:
    static constexpr const char* name = "SJF";

    void submit(Proc p) { p.rem = p.burst; rq_.push(std::move(p)); }
    bool busy()  const  { return cur_.rem > 0 || !rq_.empty(); }

    void step() {
        if (cur_.rem == 0) {
            if (rq_.empty()) { idle_++; t_++; return; }
            cur_ = rq_.top(); rq_.pop();
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
    struct SjfCompare {
        bool operator()(const Proc& a, const Proc& b) const {
            if (a.burst != b.burst) return a.burst > b.burst;
            return a.arr > b.arr;
        }
    };
    std::priority_queue<Proc, std::vector<Proc>, SjfCompare> rq_;
};
