# Nombre del ejecutable
TARGET = main

# Archivos fuente
SRC = main.cpp states/states.cpp cursor/cursor.cpp engine/cmp/CmpRender.cpp

# Compilador
CXX = g++

# Flags de compilación
CXXFLAGS = -std=c++20 -O2

# Ruta a Raylib (ajusta si lo tienes en otro sitio)
RAYLIB_PATH = C:/raylib

# Includes y librerías
INCLUDES = -I$(RAYLIB_PATH)/include
LIBS = -L$(RAYLIB_PATH)/lib -lraylib -lopengl32 -lgdi32 -lwinmm

# Comando para compilar
all:
	$(CXX) $(SRC) -o $(TARGET).exe $(CXXFLAGS) $(INCLUDES) $(LIBS)

# Limpieza
clean:
	del $(TARGET).exe