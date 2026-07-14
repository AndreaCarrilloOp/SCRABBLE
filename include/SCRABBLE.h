#ifndef SCRABBLE_H
#define SCRABBLE_H

#include <string>

void mostrarCreditos();
void mostrarEstadisticasJuego();
void limpiarPantalla();
void configurarColores();
void mostrarAyuda();
bool validarEntrada(const std::string &entrada, int minimo, int maximo);
int obtenerEntradaSegura(const std::string &mensaje, int minimo, int maximo);

#endif
