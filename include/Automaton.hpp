#pragma once

#include <set>
#include <string>
#include <vector>

#include "Transition.hpp"

/** @file
 * @brief Declara la estructura y las operaciones de un autómata con pila.
 */

/** @brief Definición de un autómata con pila de aceptación por estado final. */
class Automaton {
public:
    std::set<std::string>   Q;      /**< Conjunto de estados. */
    std::set<char>          Sigma;  /**< Alfabeto de entrada, sin epsilon. */
    std::set<char>          Gamma;  /**< Alfabeto de la pila, sin epsilon. */
    std::string             q0;     /**< Estado inicial. */
    char                    Z0{};   /**< Símbolo inicial de la pila. */
    std::set<std::string>   F;      /**< Estados de aceptación (APf). */
    std::vector<Transition> delta;  /**< Conjunto de transiciones del autómata. */

    /** @brief Comprueba que la definición cumple las restricciones formales.
     * @param error Recibe el motivo del fallo si la validación no tiene éxito.
     * @return `true` si la definición es válida; `false` en caso contrario.
     */
    bool validate(std::string& error) const;

    /** @brief Obtiene las transiciones aplicables a una configuración.
     * @param state Estado actual.
     * @param input Siguiente símbolo de entrada, o EPSILON si no hay entrada.
     * @param hasInput Indica si queda entrada por consumir.
     * @param stack Contenido de la pila, con la cima al final.
     * @return Punteros a las transiciones que pueden aplicarse.
     */
    std::vector<const Transition*> applicable(const std::string& state,
                                              char input,
                                              bool hasInput,
                                              const std::string& stack) const;

    /** @brief Devuelve una descripción textual de la definición completa.
     * @return Descripción del autómata y de sus transiciones.
     */
    std::string describe() const;
};