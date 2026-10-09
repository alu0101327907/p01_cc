#include "Simulator.hpp"

#include <deque>
#include <set>
#include <sstream>

/// Muestra la pila con la cima a la izquierda (más legible en la traza).
static std::string stackToText(const std::string& stack) {
    if (stack.empty()) return "(vacia)";
    std::string out;
    for (auto it = stack.rbegin(); it != stack.rend(); ++it) {
        if (!out.empty()) out += ' ';
        out += *it;
    }
    return out;
}

/// Resto de la entrada a partir de `pos` (o "." si ya se consumió toda).
static std::string remainingToText(const std::string& input, std::size_t pos) {
    if (pos >= input.size()) return ".";
    return input.substr(pos);
}

SimulationResult Simulator::run(const std::string& input, bool wantTrace) {
    // Nodo del árbol de cómputo: guarda su configuración, su padre y la
    // transición que se aplicó para llegar a él (para reconstruir la traza).
    struct Node {
        Configuration     cfg;
        int               parent = -1;
        const Transition* tr     = nullptr;
    };

    SimulationResult result;

    std::vector<Node>       nodes;
    std::deque<std::size_t> queue;
    std::set<Configuration> visited;

    const Configuration start{ap_.q0, 0, std::string(1, ap_.Z0)};
    nodes.push_back({start, -1, nullptr});
    visited.insert(start);
    queue.push_back(0);

    int acceptingIdx = -1;

    while (!queue.empty()) {
        const std::size_t idx = queue.front();
        queue.pop_front();

        // Copia local: el vector `nodes` puede reasignar al hacer push_back.
        const Configuration cur = nodes[idx].cfg;

        // ¿Aceptación? (APf) Entrada consumida por completo y estado final.
        if (cur.inputPos == input.size() && ap_.F.count(cur.state)) {
            acceptingIdx = static_cast<int>(idx);
            break;
        }

        if (nodes.size() >= maxConfigs_) {
            result.aborted = true;
            break;
        }

        const bool hasNext = cur.inputPos < input.size();
        const char next    = hasNext ? input[cur.inputPos] : EPSILON;

        for (const Transition* t :
             ap_.applicable(cur.state, next, hasNext, cur.stack)) {

            Configuration nc;
            nc.state    = t->to;
            nc.inputPos = cur.inputPos + (t->input == EPSILON ? 0 : 1);
            nc.stack    = cur.stack;

            // Desapilar si la transición lo indica.
            if (t->pop != EPSILON) nc.stack.pop_back();

            // Apilar: el PRIMER carácter de `push` debe quedar en la cima,
            // por eso lo apilamos en orden inverso (el último en entrar es
            // el primero de `push`).
            for (std::size_t k = t->push.size(); k-- > 0; )
                nc.stack.push_back(t->push[k]);

            if (nc.stack.size() > maxStack_) continue;

            if (visited.insert(nc).second) {
                nodes.push_back({nc, static_cast<int>(idx), t});
                queue.push_back(nodes.size() - 1);
            }
        }
    }

    result.accepted = (acceptingIdx >= 0);

    if (wantTrace) {
        // Configuración inicial
        TraceStep init;
        init.config  = start;
        init.applied = nullptr;
        result.trace.push_back(init);

        if (acceptingIdx >= 0) {
            // Reconstruimos el camino desde la raíz hasta la configuración
            // de aceptación y volcamos cada paso.
            std::vector<std::size_t> path;
            for (int i = acceptingIdx; i > 0; i = nodes[i].parent)
                path.push_back(static_cast<std::size_t>(i));

            for (auto it = path.rbegin(); it != path.rend(); ++it) {
                TraceStep s;
                s.config  = nodes[*it].cfg;
                s.applied = nodes[*it].tr;
                result.trace.push_back(s);
            }
        }
    }

    return result;
}

std::string Simulator::formatTrace(const std::string& input,
                                   const SimulationResult& r) const {
    std::ostringstream os;
    os << "Traza para la cadena: \"" << input << "\"\n";
    os << "------------------------------------------------------------\n";

    for (std::size_t k = 0; k < r.trace.size(); ++k) {
        const TraceStep& s = r.trace[k];

        if (k == 0)
            os << "Paso 0 (configuracion inicial)\n";
        else
            os << "Paso " << k << "  [aplicando " << s.applied->toString() << "]\n";

        os << "  Estado          : " << s.config.state << "\n";
        os << "  Cadena restante : " << remainingToText(input, s.config.inputPos) << "\n";
        os << "  Pila (cima izq.): " << stackToText(s.config.stack) << "\n";

        const bool hasNext = s.config.inputPos < input.size();
        const char next    = hasNext ? input[s.config.inputPos] : EPSILON;
        const auto apps =
            ap_.applicable(s.config.state, next, hasNext, s.config.stack);

        os << "  Transiciones posibles:\n";
        if (apps.empty()) {
            os << "      (ninguna)\n";
        } else {
            for (const Transition* t : apps)
                os << "      " << t->toString() << "\n";
        }
        os << "\n";
    }

    if (!r.accepted) {
        os << "No se ha encontrado ningun camino de aceptacion";
        if (r.aborted) os << " (se alcanzo el limite de exploracion)";
        os << ".\n";
    }

    return os.str();
}
