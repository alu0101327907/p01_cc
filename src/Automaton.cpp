#include "Automaton.hpp"
// comentario
#include <sstream>

namespace pda {

namespace {

std::string symStr(char c) {
    return c == EPSILON ? "." : std::string(1, c);
}

} // namespace

bool Automaton::validate(std::string& error) const {
    if (Q.empty())     { error = "El conjunto de estados Q esta vacio.";           return false; }
    if (Gamma.empty()) { error = "El alfabeto de pila Gamma esta vacio.";          return false; }

    if (!Q.count(q0)) {
        error = "El estado inicial '" + q0 + "' no pertenece a Q.";
        return false;
    }
    if (!Gamma.count(Z0)) {
        error = "El simbolo inicial de pila '" + symStr(Z0) + "' no pertenece a Gamma.";
        return false;
    }
    for (const auto& f : F) {
        if (!Q.count(f)) {
            error = "El estado final '" + f + "' no pertenece a Q.";
            return false;
        }
    }
    for (char c : Sigma)
        if (c == '.') { error = "Sigma no puede contener el simbolo epsilon ('.')."; return false; }
    for (char c : Gamma)
        if (c == '.') { error = "Gamma no puede contener el simbolo epsilon ('.')."; return false; }

    for (const auto& t : delta) {
        if (!Q.count(t.from)) {
            error = "Transicion con estado origen desconocido: " + t.from;
            return false;
        }
        if (!Q.count(t.to)) {
            error = "Transicion con estado destino desconocido: " + t.to;
            return false;
        }
        if (t.input != EPSILON && !Sigma.count(t.input)) {
            error = std::string("Transicion con simbolo de entrada '") + t.input +
                    "' que no pertenece a Sigma.";
            return false;
        }
        if (t.pop != EPSILON && !Gamma.count(t.pop)) {
            error = std::string("Transicion con simbolo de pila '") + t.pop +
                    "' que no pertenece a Gamma.";
            return false;
        }
        for (char c : t.push) {
            if (!Gamma.count(c)) {
                error = std::string("Transicion que apila '") + c +
                        "', que no pertenece a Gamma.";
                return false;
            }
        }
    }
    return true;
}

std::vector<const Transition*> Automaton::applicable(const std::string& state,
                                                     char input,
                                                     bool hasInput,
                                                     const std::string& stack) const {
    std::vector<const Transition*> out;
    const char top = stack.empty() ? EPSILON : stack.back();

    for (const auto& t : delta) {
        if (t.from != state) continue;

        // Símbolo de entrada: o coincide, o es ε.
        if (t.input != EPSILON) {
            if (!hasInput || t.input != input) continue;
        }

        // Símbolo desapilado: o coincide con la cima, o es ε.
        if (t.pop != EPSILON) {
            if (stack.empty() || t.pop != top) continue;
        }

        out.push_back(&t);
    }
    return out;
}

std::string Automaton::describe() const {
    std::ostringstream os;
    os << "Tipo de aceptacion: APf (por estado final)\n";
    os << "Q     = {";  for (const auto& s : Q) os << ' ' << s; os << " }\n";
    os << "Sigma = {";  for (char c : Sigma)     os << ' ' << c; os << " }\n";
    os << "Gamma = {";  for (char c : Gamma)     os << ' ' << c; os << " }\n";
    os << "q0    = " << q0 << "\n";
    os << "Z0    = " << Z0 << "\n";
    os << "F     = {";  for (const auto& s : F) os << ' ' << s; os << " }\n";
    os << "delta:\n";
    for (const auto& t : delta) os << "    " << t.toString() << "\n";
    return os.str();
}

} // namespace pda