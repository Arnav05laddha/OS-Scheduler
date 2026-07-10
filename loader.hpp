#pragma once
#include "process.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

inline std::vector<Proc> load_csv(const std::string& path) {
    std::ifstream f(path);
    if (!f) throw std::runtime_error("Cannot open: " + path);
    std::string line;
    std::getline(f, line); // header
    std::vector<Proc> ps;
    while (std::getline(f, line)) {
        if (line.empty()) continue;
        std::istringstream ss(line);
        std::string tok;
        Proc p{}; p.start = -1;
        try {
            if (!std::getline(ss, tok, ',')) continue;
            p.pid = std::stoi(tok);
            if (!std::getline(ss, tok, ',')) continue;
            p.arr = std::stoi(tok);
            if (!std::getline(ss, tok, ',')) continue;
            p.burst = std::stoi(tok);
            if (!std::getline(ss, tok, ',')) continue;
            p.prio = std::stoi(tok);
            ps.push_back(p);
        } catch (const std::exception&) {
            // skip malformed or empty line safely
            continue;
        }
    }
    return ps;
}
