#pragma once

#include <set>
#include <string>
#include <vector>

#include "pda/Transition.hpp"

namespace pda {

class Automaton {
public:
    std::set<std::string>   Q;      // conjunto de estados
    std::set<char>          Sigma;  // alfabeto de entrada
    std::set<char>          Gamma;  // alfabeto de pila
    std::string             q0;     // estado inicial
    char                    Z0{};   // símbolo inicial de la pila
    std::set<std::string>   F;      // estados finales (APf)
    std::vector<Transition> delta;  // función de transición

    /// Verifica que la definición cumple las restricciones formales.
    bool validate(std::string& error) const;

    /// Transiciones aplicables desde `state`, con `input` como siguiente
    /// símbolo (o EPSILON si no queda entrada) y `stack` (cima al final).
    std::vector<const Transition*> applicable(const std::string& state,
                                              char input,
                                              bool hasInput,
                                              const std::string& stack) const;

    std::string describe() const;
};

} // namespace pda