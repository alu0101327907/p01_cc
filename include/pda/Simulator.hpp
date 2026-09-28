#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "pda/Automaton.hpp"

namespace pda {

/// Configuración instantánea del autómata.
struct Configuration {
    std::string state;
    std::size_t inputPos = 0;
    std::string stack;    // cima al FINAL de la cadena

    bool operator<(const Configuration& o) const {
        if (state    != o.state)    return state    < o.state;
        if (inputPos != o.inputPos) return inputPos < o.inputPos;
        return stack < o.stack;
    }
};

struct TraceStep {
    Configuration     config;
    const Transition* applied = nullptr;   // nullptr en el paso inicial
};

struct SimulationResult {
    bool accepted = false;
    bool aborted  = false;                 // se alcanzó el límite de exploración
    std::vector<TraceStep> trace;
};

class Simulator {
public:
    explicit Simulator(const Automaton& ap,
                       std::size_t maxConfigs = 200000,
                       std::size_t maxStack   = 5000)
        : ap_(ap), maxConfigs_(maxConfigs), maxStack_(maxStack) {}

    SimulationResult run(const std::string& input, bool wantTrace);

    std::string formatTrace(const std::string& input,
                            const SimulationResult& r) const;

private:
    const Automaton& ap_;
    std::size_t      maxConfigs_;
    std::size_t      maxStack_;
};

} // namespace pda