#include "ConfigParser.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

/// @brief Elimina espacios, tabs y saltos de línea de ambos extremos de la cadena.
/// @param s Cadena de entrada.
/// @return Copia de @p s sin espacios iniciales ni finales. Cadena vacía si @p s es solo espacios.
static std::string trim(const std::string& s) {
  const auto b = s.find_first_not_of(" \t\r\n");
  if (b == std::string::npos) return "";
  const auto e = s.find_last_not_of(" \t\r\n");
  return s.substr(b, e - b + 1);
}

/// @brief Divide una cadena en tokens separados por cualquier cantidad de espacios en blanco.
/// @param s Cadena de entrada.
/// @return Vector con los tokens no vacíos, en el orden en que aparecen.
///         Vector vacío si @p s es vacía o solo contiene espacios.
static std::vector<std::string> splitTokens(const std::string& s) {
  std::vector<std::string> out;
  std::istringstream iss(s);
  std::string tok;
  while (iss >> tok) out.push_back(tok);
  return out;
}

/// @brief Convierte un token en un símbolo interno de un único carácter.
///
/// El token "." se interpreta como epsilon (@ref EPSILON). Cualquier otro
/// token debe tener longitud exactamente 1.
///
/// @param tok Token del fichero (ej. "a", "Z", ".").
/// @param what Descripción del contexto, usada solo en el mensaje de error
///             (ej. "simbolo de entrada", "simbolo de pila").
/// @return El carácter del símbolo, o EPSILON si @p tok es ".".
/// @throws std::runtime_error Si @p tok no es "." ni un único carácter.
static char toSymbol(const std::string& tok, const std::string& what) {
    if (tok == ".") return EPSILON;
    if (tok.size() != 1)
        throw std::runtime_error("El " + what + " '" + tok +
                                 "' debe estar formado por un unico caracter.");
    return tok[0];
}

Automaton loadAutomaton(const std::string& filename) {
    std::ifstream in(filename);
    if (!in)
        throw std::runtime_error("No se puede abrir el fichero de configuracion: " +
                                 filename);

    // 1) Leer todas las líneas significativas (sin comentarios ni líneas vacías).
    std::vector<std::string> data;
    std::string line;
    while (std::getline(in, line)) {
        const auto hash = line.find('#');
        if (hash != std::string::npos) line = line.substr(0, hash);
        line = trim(line);
        if (!line.empty()) data.push_back(line);
    }

    if (data.size() < 7)
        throw std::runtime_error(
            "Fichero de configuracion incompleto: se esperaban 6 cabeceras "
            "(Q, Sigma, Gamma, q0, Z0, F) y al menos una transicion.");

    Automaton ap;
    std::size_t i = 0;

    // Q
    for (const auto& s : splitTokens(data[i])) ap.Q.insert(s);
    ++i;

    // Sigma
    for (const auto& s : splitTokens(data[i])) {
        if (s == ".") throw std::runtime_error("Sigma no puede contener epsilon ('.').");
        if (s.size() != 1)
            throw std::runtime_error("El simbolo de Sigma '" + s +
                                     "' debe ser un unico caracter.");
        ap.Sigma.insert(s[0]);
    }
    ++i;

    // Gamma
    for (const auto& s : splitTokens(data[i])) {
        if (s == ".") throw std::runtime_error("Gamma no puede contener epsilon ('.').");
        if (s.size() != 1)
            throw std::runtime_error("El simbolo de Gamma '" + s +
                                     "' debe ser un unico caracter.");
        ap.Gamma.insert(s[0]);
    }
    ++i;

    // q0
    {
        const auto tok = splitTokens(data[i]);
        if (tok.size() != 1)
            throw std::runtime_error("El estado inicial debe ser un unico estado.");
        ap.q0 = tok[0];
    }
    ++i;

    // Z0
    {
        const auto tok = splitTokens(data[i]);
        if (tok.size() != 1 || tok[0] == "." || tok[0].size() != 1)
            throw std::runtime_error(
                "El simbolo inicial de pila debe ser un unico caracter de Gamma.");
        ap.Z0 = tok[0][0];
    }
    ++i;

    // F (APf)
    for (const auto& s : splitTokens(data[i])) ap.F.insert(s);
    ++i;

    // Transiciones
    for (; i < data.size(); ++i) {
        const auto tok = splitTokens(data[i]);
        if (tok.size() != 5)
            throw std::runtime_error("Transicion mal formada: '" + data[i] +
                                     "' (se esperaban 5 campos).");

        Transition t;
        t.from  = tok[0];
        t.input = toSymbol(tok[1], "simbolo de entrada");
        t.pop   = toSymbol(tok[2], "simbolo de pila");
        t.to    = tok[3];

        if (tok[4] == ".") {
            t.push.clear();
        } else {
            if (tok[4].find('.') != std::string::npos)
                throw std::runtime_error(
                    "La cadena apilada no puede contener '.' (epsilon).");
            t.push = tok[4];
        }
        ap.delta.push_back(t);
    }

    std::string error;
    if (!ap.validate(error))
        throw std::runtime_error("Definicion del automata incorrecta: " + error);

    return ap;
}
