#include "Transition.hpp"

/// Representación legible de un símbolo (ε se muestra como ".").
static std::string sym(char c) {
    return c == EPSILON ? std::string(".") : std::string(1, c);
}

std::string Transition::toString() const {
    return "(" + from + ", " + sym(input) + ", " + sym(pop) + ") -> (" +
           to + ", " + (push.empty() ? "." : push) + ")";
}
