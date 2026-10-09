#pragma once

#include <string>

/** @file
 * @brief Define los símbolos y las transiciones de un autómata con pila.
 */

/** @brief Valor interno que representa la palabra vacía (epsilon). */
inline constexpr char EPSILON = '\0';

/** @brief Transición de un autómata con pila.
 *
 * Representa la regla (from, input, pop) -> (to, push).
 * `input` o `pop` iguales a EPSILON indican que no se consume entrada o no se
 * desapila, respectivamente. Una cadena `push` vacía indica que no se apila.
 * El primer carácter de `push` queda en la cima de la pila.
 */
struct Transition {
    std::string from;       /**< Estado de origen. */
    char        input = EPSILON; /**< Símbolo de entrada que se consume. */
    char        pop   = EPSILON; /**< Símbolo que se desapila. */
    std::string to;         /**< Estado de destino. */
    std::string push;       /**< Símbolos que se apilan, en orden de cima a base. */

    /** @brief Devuelve una representación legible de la transición.
     * @return Transición con el formato `(origen, entrada, desapilar) -> (destino, apilar)`.
     */
    std::string toString() const;
};