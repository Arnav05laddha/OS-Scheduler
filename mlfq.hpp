#pragma once
#include "process.hpp"
#include <algorithm>
#include <deque>
#include <unordered_map>
#include <vector>

class MLFQ : public Scheduler<MLFQ> {
public:
    static constexpr const char* name = "MLFQ";
    explicit MLFQ(int lvl = 3, int bq = 4, int age = 20)
        : lvls_(lvl), base_q_(bq), age_(age), qs_(lvl) {}

    void submit(Proc p) {
        p.rem = p.burst;
        qs_[0].push_back(std::move(p));
        in_lvl_[p.pid] = 0;
        wait_at_[p.pid] = t_;
    }

    bool busy() const {
        if (has_) return true;
        for (auto& q : qs_) if (!q.empty()) return true;
        return false;
    }

    void step() {
        do_aging();
        if (!has_) pick_next();
        if (!has_) { idle_++; t_++; return; }

        cur_.rem--; run_++; t_++;
        int q = base_q_ << cur_lvl_;

        if (cur_.rem == 0) {
            cur_.finish = t_;
            gantt_.push_back({cur_.pid, seg_, t_});
            done_.push_back(cur_); has_ = false;
        } else if (run_ >= q) {
            gantt_.push_back({cur_.pid, seg_, t_});
            int nl = std::min(cur_lvl_ + 1, lvls_ - 1);
            qs_[nl].push_back(cur_);
            in_lvl_[cur_.pid] = nl;
            wait_at_[cur_.pid] = t_;
            has_ = false;
        }
    }

private:
    int lvls_, base_q_, age_;
    std::vector<std::deque<Proc>>   qs_;
    std::unordered_map<int,int>     in_lvl_, wait_at_;
    int cur_lvl_ = 0;

    void pick_next() {
        for (int l = 0; l < lvls_; l++) {
            if (!qs_[l].empty()) {
                cur_ = qs_[l].front(); qs_[l].pop_front();
                in_lvl_.erase(cur_.pid); wait_at_.erase(cur_.pid);
                if (cur_.start == -1) cur_.start = t_;
                cur_lvl_ = l; seg_ = t_; run_ = 0; has_ = true;
                return;
            }
        }
    }

    // Promote processes that waited >= age_ ticks in any queue below level 0
    void do_aging() {
        for (int l = 1; l < lvls_; l++) {
            for (int i = 0; i < (int)qs_[l].size(); ) {
                auto& p = qs_[l][i];
                if (t_ - wait_at_[p.pid] >= age_) {
                    Proc pp = std::move(p);
                    qs_[l].erase(qs_[l].begin() + i);
                    int nl = l - 1;
                    qs_[nl].push_front(std::move(pp));
                    in_lvl_[pp.pid] = nl;
                    wait_at_[pp.pid] = t_;
                } else { i++; }
            }
        }
    }
};
