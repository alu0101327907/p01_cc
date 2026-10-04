#pragma once

#include <string>

#include "Automaton.hpp"

namespace pda {

/// Lee un fichero de configuración y construye el autómata.
/// Lanza std::runtime_error si el fichero no es válido.
Automaton loadAutomaton(const std::string& filename);

} // namespace pda