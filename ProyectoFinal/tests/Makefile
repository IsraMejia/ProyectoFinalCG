# Makefile for Train Animation Bug Tests
# Includes bug condition test and preservation property tests

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra -I../include -I../glm -I..

# Linker flags
LDFLAGS = -L../lib

# Libraries
LIBS = -lglew32 -lglfw3 -lopengl32 -lgdi32

# Source files for bug condition test
BUG_SOURCES = train_animation_bug_condition_test.cpp \
              ../Integrantes/Isra/animacion_KF.cpp \
              ../Integrantes/Isra/tren.cpp \
              ../dependencias/Model.cpp \
              ../dependencias/Mesh.cpp \
              ../dependencias/Material.cpp

# Source files for preservation tests (standalone, no dependencies)
PRESERVATION_SOURCES = preservation_property_tests.cpp

# Object files
BUG_OBJECTS = $(BUG_SOURCES:.cpp=.o)

# Output executables
BUG_TARGET = train_animation_bug_test.exe
PRESERVATION_TARGET = preservation_tests.exe
MINIMAL_TARGET = train_animation_bug_minimal_test.exe

# Default target - build all tests
all: $(BUG_TARGET) $(PRESERVATION_TARGET) $(MINIMAL_TARGET)

# Link the bug condition test executable
$(BUG_TARGET): $(BUG_OBJECTS)
	$(CXX) $(BUG_OBJECTS) -o $(BUG_TARGET) $(LDFLAGS) $(LIBS)

# Link the preservation tests executable (standalone, no external dependencies)
$(PRESERVATION_TARGET): $(PRESERVATION_SOURCES)
	$(CXX) $(CXXFLAGS) $(PRESERVATION_SOURCES) -o $(PRESERVATION_TARGET)

# Link the minimal test executable (standalone, no external dependencies)
$(MINIMAL_TARGET): train_animation_bug_minimal_test.cpp
	$(CXX) $(CXXFLAGS) train_animation_bug_minimal_test.cpp -o $(MINIMAL_TARGET)

# Compile source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the bug condition test
test-bug: $(BUG_TARGET)
	./$(BUG_TARGET)

# Run the preservation tests
test-preservation: $(PRESERVATION_TARGET)
	./$(PRESERVATION_TARGET)

# Run the minimal test
test-minimal: $(MINIMAL_TARGET)
	./$(MINIMAL_TARGET)

# Run all tests
test: test-minimal test-preservation
	@echo ""
	@echo "=========================================="
	@echo "ALL TESTS COMPLETED"
	@echo "=========================================="
	@echo ""
	@echo "Bug condition test: Demonstrates the bug exists"
	@echo "Preservation tests: Verify baseline behavior to preserve"

# Clean build artifacts
clean:
	rm -f $(BUG_OBJECTS) $(BUG_TARGET) $(PRESERVATION_TARGET) $(MINIMAL_TARGET)

# Phony targets
.PHONY: all test test-bug test-preservation test-minimal clean
