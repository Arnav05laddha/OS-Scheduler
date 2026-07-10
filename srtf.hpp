#pragma once
#include "process.hpp"
#include <queue>

class SRTF : public Scheduler<SRTF> {
public:
    static constexpr const char* name = "SRTF";

    void submit(Proc p) { p.rem = p.burst; rq_.push(std::move(p)); }
    bool busy()  const  { return has_ || !rq_.empty(); }

    void step() {
        if (rq_.empty() && !has_) { idle_++; t_++; return; }

        if (!rq_.empty() && (!has_ || rq_.top().rem < cur_.rem)) {
            if (has_) {
                gantt_.push_back({cur_.pid, seg_, t_});
                rq_.push(cur_);
            }
            cur_ = rq_.top(); rq_.pop();
            if (cur_.start == -1) cur_.start = t_;
            seg_ = t_; has_ = true;
        }

        cur_.rem--;
        t_++;
        if (cur_.rem == 0) {
            cur_.finish = t_;
            gantt_.push_back({cur_.pid, seg_, t_});
            done_.push_back(cur_);
            has_ = false;
        }
    }

private:
    struct SrtfCompare {
        bool operator()(const Proc& a, const Proc& b) const {
            if (a.rem != b.rem) return a.rem > b.rem;
            return a.arr > b.arr;
        }
    };
    std::priority_queue<Proc, std::vector<Proc>, SrtfCompare> rq_;
};
