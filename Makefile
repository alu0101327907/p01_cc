# ============================================================
#  Makefile - Simulador de Autómata con Pila (APf)
# ============================================================

CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -O2 -Iinclude
LDFLAGS  :=

TARGET   := pda
SRC_DIR  := src
BUILD    := build

SRCS     := $(wildcard $(SRC_DIR)/*.cpp)
OBJS     := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD)/%.o,$(SRCS))
DEPS     := $(OBJS:.o=.d)

# ------------------------------------------------------------
#  Reglas
# ------------------------------------------------------------

.PHONY: all clean run rebuild help

all: $(TARGET)

# Enlazado final -> ejecutable en la raíz
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "  [OK] Ejecutable generado: $@"

# Compilación de cada .cpp -> .o (con generación de dependencias .d)
$(BUILD)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@
	@echo "  [CC] $<"

# Carga las dependencias de headers generadas por -MMD
-include $(DEPS)

# ------------------------------------------------------------
#  Utilidades
# ------------------------------------------------------------

run: all
	./$(TARGET) -config examples/APf-1.txt -trace y -in examples/cadenas.txt

clean:
	rm -f $(TARGET)
	rm -rf $(BUILD)
	@echo "  [CLEAN] $(TARGET) y $(BUILD)/ eliminados"

rebuild: clean all

help:
	@echo "Objetivos disponibles:"
	@echo "  make            -> compila el proyecto (genera ./pda)"
	@echo "  make run        -> compila y ejecuta con los ejemplos"
	@echo "  make clean      -> borra ./pda y build/"
	@echo "  make rebuild    -> clean + all"