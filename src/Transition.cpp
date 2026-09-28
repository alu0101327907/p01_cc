#include "pda/Transition.hpp"

namespace pda {

namespace {

/// Representación legible de un símbolo (ε se muestra como ".").
std::string sym(char c) {
    return c == EPSILON ? std::string(".") : std::string(1, c);
}

} // namespace

std::string Transition::toString() const {
    return "(" + from + ", " + sym(input) + ", " + sym(pop) + ") -> (" +
           to + ", " + (push.empty() ? "." : push) + ")";
}

} // namespace pda