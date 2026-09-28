#pragma once

#include <string>

namespace pda {

/// Valor interno que representa la palabra vacía (ε).
inline constexpr char EPSILON = '\0';

/**
 * Transición de un autómata con pila:
 *      (from, input, pop) -> (to, push)
 * que representa  (to, push) ∈ δ(from, input, pop).
 *
 * - `input` == EPSILON  indica que no se consume entrada.
 * - `pop`   == EPSILON  indica que no se desapila nada.
 * - `push`  vacío       indica que no se apila nada.
 *   El primer carácter de `push` queda en la cima de la pila.
 */
struct Transition {
    std::string from;
    char        input = EPSILON;
    char        pop   = EPSILON;
    std::string to;
    std::string push;

    std::string toString() const;
};

} // namespace pda