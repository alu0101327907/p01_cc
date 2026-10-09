#pragma once

#include <string>

#include "Automaton.hpp"

/** @file
 * @brief Declara la lectura y validación de autómatas desde archivos.
 */

/** @brief Carga y valida un autómata con pila desde un archivo de texto.
///
/// El fichero debe contener, en este orden, seis cabeceras (una por línea):
///   - Q:     estados, separados por espacios.
///   - Sigma: alfabeto de entrada (símbolos de un único carácter, sin '.').
///   - Gamma: alfabeto de pila (símbolos de un único carácter, sin '.').
///   - q0:    estado inicial (un único token).
///   - Z0:    símbolo inicial de pila (un único carácter de Gamma).
///   - F:     estados finales, separados por espacios.
///
/// A continuación, una o más transiciones con el formato:
///   from input pop to push
/// donde:
///   - input y pop pueden ser '.' para representar epsilon.
///   - push puede ser '.' (no apilar nada) o una cadena de símbolos de Gamma
///     sin '.' en su interior.
///
/// Se ignoran las líneas vacías y todo lo que sigue al carácter '#'.
///
 * @param filename Ruta del fichero de configuración.
 * @return Autómata inicializado y validado.
 * @throws std::runtime_error Si el fichero no se puede abrir, está mal formado,
 * contiene símbolos inválidos o no supera la validación del autómata.
 */
Automaton loadAutomaton(const std::string& filename);