#!/usr/bin/env bash
# build.sh: Script sencillo para compilar el proyecto CMake ubicado en la carpeta 'sources'.
# Uso: ./build.sh

set -e

# Limpiar y crear carpeta build
rm -rf build && mkdir build

# Entrar en build
echo "-> Entrando en build/"
cd build

# Configurar el proyecto con CMake
echo "-> Ejecutando cmake ../sources"
cmake ../sources

# Compilar el proyecto
echo "-> Ejecutando cmake --build ."
cmake --build .

# Volver al directorio original
echo "-> Volviendo al directorio raíz"
cd ..

echo "Compilación completada correctamente."