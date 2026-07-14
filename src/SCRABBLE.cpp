#include "SCRABBLE.h"
#include "Consola.h"
#include <iostream>
#include <sstream>

void mostrarCreditos()
{
    std::cout << "SCRABBLE en C++ - Andrea Carrillo\n";
}
void mostrarEstadisticasJuego()
{
    std::cout << "Tablero 15x15, atril de 6 fichas y diccionario local.\n";
}
void limpiarPantalla() { Consola::limpiarPantalla(); }
void configurarColores() {}
void mostrarAyuda()
{
    std::cout << "Forme palabras conectadas y use las casillas de bono para sumar puntos.\n";
}
bool validarEntrada(const std::string &entrada, int minimo, int maximo)
{
    std::istringstream flujo(entrada);
    int valor;
    char extra;
    return (flujo >> valor) && !(flujo >> extra) && valor >= minimo && valor <= maximo;
}
int obtenerEntradaSegura(const std::string &mensaje, int minimo, int maximo)
{
    std::string entrada;
    while (true)
    {
        std::cout << mensaje;
        if (!std::getline(std::cin, entrada))
            return maximo;
        if (validarEntrada(entrada, minimo, maximo))
            return std::stoi(entrada);
        std::cout << "Entrada inválida.\n";
    }
}
