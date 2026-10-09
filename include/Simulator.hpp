#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "Automaton.hpp"

/** @file
 * @brief Declara las configuraciones, resultados y simulador del autómata.
 */

/** @brief Configuración instantánea del autómata durante una ejecución. */
struct Configuration {
    std::string state;        /**< Estado actual. */
    std::size_t inputPos = 0; /**< Posición del siguiente símbolo de entrada. */
    std::string stack;        /**< Pila con la cima al final de la cadena. */

    /** @brief Compara configuraciones para almacenarlas en conjuntos ordenados.
     * @param other Configuración que se compara con esta.
     * @return `true` si esta configuración precede a `other` lexicográficamente.
     */
    bool operator<(const Configuration& other) const {
        if (state    != other.state)    return state    < other.state;
        if (inputPos != other.inputPos) return inputPos < other.inputPos;
        return stack < other.stack;
    }
};

/** @brief Paso de una traza de ejecución. */
struct TraceStep {
    Configuration     config;  /**< Configuración alcanzada en este paso. */
    const Transition* applied = nullptr; /**< Transición aplicada; nula al inicio. */
};

/** @brief Resultado de simular una cadena. */
struct SimulationResult {
    bool accepted = false; /**< Indica si se encontró un camino de aceptación. */
    bool aborted  = false; /**< Indica si se alcanzó el límite de exploración. */
    std::vector<TraceStep> trace; /**< Camino de aceptación, si se solicitó. */
};

/** @brief Explora las ejecuciones posibles de un autómata con pila. */
class Simulator {
public:
    /** @brief Crea un simulador con límites para la exploración y la pila.
     * @param automaton Autómata que se va a simular; debe vivir más que el simulador.
     * @param maxConfigs Máximo de configuraciones exploradas.
     * @param maxStack Altura máxima permitida para la pila.
     */
    explicit Simulator(const Automaton& automaton,
                       std::size_t maxConfigs = 200000,
                       std::size_t maxStack   = 5000)
        : ap_(automaton), maxConfigs_(maxConfigs), maxStack_(maxStack) {}

    /** @brief Simula una cadena y, opcionalmente, guarda la traza aceptante.
     * @param input Cadena que se procesa.
     * @param wantTrace Indica si se reconstruye la traza de aceptación.
     * @return Resultado de la simulación.
     */
    SimulationResult run(const std::string& input, bool wantTrace);

    /** @brief Formatea la traza de una simulación para mostrarla.
     * @param input Cadena que se simuló.
     * @param result Resultado producido por `run`.
     * @return Traza en texto, incluidos los estados y las transiciones posibles.
     */
    std::string formatTrace(const std::string& input,
                            const SimulationResult& result) const;

private:
    const Automaton& ap_;
    std::size_t      maxConfigs_;
    std::size_t      maxStack_;
};