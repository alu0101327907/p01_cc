#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Automaton.hpp"
#include "ConfigParser.hpp"
#include "Simulator.hpp"

static void usage(const char* prog) {
    std::cout
        << "Uso: " << prog << " -config <f> -trace <y|n> [-in <f>] [-out <f>]\n"
           "  -config <f>   Fichero con la definicion del automata con pila.\n"
           "  -trace  <y|n> Muestra (y) o no (n) la traza de ejecucion.\n"
           "  -in     <f>   Fichero con las cadenas de entrada (opcional).\n"
           "                Si no se indica, se leen por teclado.\n"
           "  -out    <f>   Fichero donde escribir la traza (opcional).\n"
           "                Si no se indica, la traza se muestra por pantalla.\n";
}

/// Elimina el '\r' final que dejan algunos ficheros con finales de línea CRLF.
static void trimCR(std::string& s) {
    if (!s.empty() && s.back() == '\r') s.pop_back();
}

int main(int argc, char** argv) {
    std::string configFile, inFile, outFile;
    bool trace     = false;
    bool hasConfig = false;
    bool hasIn     = false;
    bool hasOut    = false;

    // ------------------------------------------------------------------
    // 1) Analizar la línea de comandos
    // ------------------------------------------------------------------
    for (int i = 1; i < argc; ++i) {
        const std::string a = argv[i];
        if (a == "-config" && i + 1 < argc) {
            configFile = argv[++i];
            hasConfig  = true;
        } else if (a == "-trace" && i + 1 < argc) {
            const std::string v = argv[++i];
            trace = (v == "y" || v == "Y");
        } else if (a == "-in" && i + 1 < argc) {
            inFile = argv[++i];
            hasIn  = true;
        } else if (a == "-out" && i + 1 < argc) {
            outFile = argv[++i];
            hasOut  = true;
        } else {
            usage(argv[0]);
            return 1;
        }
    }

    if (!hasConfig) {
        usage(argv[0]);
        return 1;
    }

    // ------------------------------------------------------------------
    // 2) Cargar y validar el autómata
    // ------------------------------------------------------------------
    Automaton ap;
    try {
        ap = loadAutomaton(configFile);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 2;
    }

    std::cout << "Automata cargado correctamente (aceptacion por estado final, APf).\n";

    // ------------------------------------------------------------------
    // 3) Obtener las cadenas de entrada
    // ------------------------------------------------------------------
    std::vector<std::string> cadenas;
    if (hasIn) {
        std::ifstream f(inFile);
        if (!f) {
            std::cerr << "Error: no se puede abrir el fichero de entrada '"
                      << inFile << "'\n";
            return 2;
        }
        std::string l;
        while (std::getline(f, l)) {
            trimCR(l);
            cadenas.push_back(l);
        }
    } else {
        std::cout << "Introduzca cadenas (una por linea, Ctrl+D para terminar):\n";
        std::string l;
        while (std::getline(std::cin, l)) {
            trimCR(l);
            cadenas.push_back(l);
        }
    }

    // ------------------------------------------------------------------
    // 4) Preparar el destino de la traza
    // ------------------------------------------------------------------
    std::ofstream outStream;
    std::ostream* traceOut = &std::cout;
    if (hasOut) {
        outStream.open(outFile);
        if (!outStream) {
            std::cerr << "Error: no se puede crear el fichero de salida '"
                      << outFile << "'\n";
            return 2;
        }
        traceOut = &outStream;
    }

    // ------------------------------------------------------------------
    // 5) Simular cada cadena
    // ------------------------------------------------------------------
    Simulator sim(ap);

    for (const auto& w : cadenas) {
        // Aviso si la cadena contiene símbolos fuera de Sigma.
        for (char c : w) {
            if (!ap.Sigma.count(c)) {
                std::cout << "Aviso: la cadena contiene '" << c
                          << "', que no pertenece a Sigma.\n";
                break;
            }
        }

        const auto r = sim.run(w, trace);

        std::cout << "Cadena \"" << w << "\": "
                  << (r.accepted ? "PERTENECE" : "NO PERTENECE")
                  << " al lenguaje.\n";
        if (r.aborted)
            std::cout << "  (aviso: se alcanzo el limite de exploracion)\n";

        if (trace) {
            *traceOut << sim.formatTrace(w, r);
            *traceOut << "Resultado: "
                      << (r.accepted ? "ACEPTADA" : "RECHAZADA") << "\n\n";
        }
    }

    return 0;
}