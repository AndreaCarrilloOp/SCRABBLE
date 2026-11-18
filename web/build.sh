#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
out="dist"
mkdir -p "$out"
cp web/index.html web/styles.css web/app.js "$out/"
sources=(
  web/bridge.cpp
  src/Consola.cpp src/Cuadricula.cpp src/DiccionarioArchivo.cpp
  src/Ficha.cpp src/HistorialPalabras.cpp src/Jugador.cpp
  src/ListaFicha.cpp src/ListaJugador.cpp src/ListaPalabra.cpp
  src/Palabra.cpp src/Tablero.cpp src/ValidadorPalabras.cpp
)
em++ -std=c++17 -O2 -fexceptions -Iinclude "${sources[@]}" \
  --no-entry --embed-file Datos.txt@/Datos.txt \
  -sMODULARIZE=1 -sEXPORT_NAME=createScrabble \
  -sENVIRONMENT=web,node -sALLOW_MEMORY_GROWTH=1 \
  -sEXPORTED_RUNTIME_METHODS=ccall \
  '-sEXPORTED_FUNCTIONS=["_scrabble_state","_scrabble_start","_scrabble_play","_scrabble_exchange","_scrabble_pass","_scrabble_finish"]' \
  -o "$out/engine.js"
