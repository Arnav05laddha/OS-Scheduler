#pragma once
#include "process.hpp"
#include <queue>

class Priority : public Scheduler<Priority> {
public:
    static constexpr const char* name = "Priority";
    explicit Priority(bool pre = false) : pre_(pre) {}

    void submit(Proc p) { p.rem = p.burst; rq_.push(std::move(p)); }
    bool busy()  const  { return has_ || !rq_.empty(); }

    void step() {
        if (pre_) step_pre();
        else      step_np();
    }

private:
    bool pre_;
    struct PriorityCompare {
        bool operator()(const Proc& a, const Proc& b) const {
            if (a.prio != b.prio) return a.prio > b.prio;
            return a.arr > b.arr;
        }
    };
    std::priority_queue<Proc, std::vector<Proc>, PriorityCompare> rq_;

    void step_np() {
        if (!has_) {
            if (rq_.empty()) { idle_++; t_++; return; }
            cur_ = rq_.top(); rq_.pop();
            cur_.start = t_; has_ = true;
        }
        cur_.rem--; t_++;
        if (cur_.rem == 0) {
            cur_.finish = t_;
            gantt_.push_back({cur_.pid, cur_.start, t_});
            done_.push_back(cur_); has_ = false;
        }
    }

    void step_pre() {
        if (rq_.empty() && !has_) { idle_++; t_++; return; }

        if (!rq_.empty() && (!has_ || rq_.top().prio < cur_.prio)) {
            if (has_) {
                gantt_.push_back({cur_.pid, seg_, t_});
                rq_.push(cur_);
            }
            cur_ = rq_.top(); rq_.pop();
            if (cur_.start == -1) cur_.start = t_;
            seg_ = t_; has_ = true;
        }
        cur_.rem--; t_++;
        if (cur_.rem == 0) {
            cur_.finish = t_;
            gantt_.push_back({cur_.pid, seg_, t_});
            done_.push_back(cur_); has_ = false;
        }
    }
};
