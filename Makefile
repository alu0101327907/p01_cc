# ============================================================
#  Makefile - Simulador de Autómata con Pila (APf)
# ============================================================

CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -O2 -Iinclude
LDFLAGS  :=

TARGET   := pda
BUILD    := build
SRC_DIR  := src
INC_DIR  := include

SRCS     := $(wildcard $(SRC_DIR)/*.cpp)
OBJS     := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD)/%.o,$(SRCS))
DEPS     := $(OBJS:.o=.d)

# ------------------------------------------------------------
#  Reglas
# ------------------------------------------------------------

.PHONY: all clean run rebuild help

all: $(BUILD)/$(TARGET)

# Enlazado final
$(BUILD)/$(TARGET): $(OBJS)
	@mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "  [OK] Ejecutable generado: $@"

# Compilación de cada .cpp -> .o (genera dependencias .d)
$(BUILD)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@
	@echo "  [CC] $<"

# Incluye las dependencias generadas automáticamente
-include $(DEPS)

# ------------------------------------------------------------
#  Utilidades
# ------------------------------------------------------------

# Ejecuta el simulador con los ejemplos por defecto
run: all
	./$(BUILD)/$(TARGET) -config examples/anbn.pda -trace y -in examples/cadenas.txt

# Borra los artefactos de compilación
clean:
	rm -rf $(BUILD)
	@echo "  [CLEAN] $(BUILD)/ eliminado"

# Recompila desde cero
rebuild: clean all

help:
	@echo "Objetivos disponibles:"
	@echo "  make            -> compila el proyecto (genera build/pda)"
	@echo "  make run        -> compila y ejecuta con los ejemplos"
	@echo "  make clean      -> borra build/"
	@echo "  make rebuild    -> clean + all"