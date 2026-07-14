#include "Consola.h"
#include <cstdlib>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

void Consola::configurarConsola()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    SetConsoleTitleA("SCRABBLE - Juego en C++");
#endif
}

void Consola::limpiarPantalla()
{
#ifdef _WIN32
    std::system("cls");
#else
    std::cout << "\033[2J\033[H";
#endif
}

void Consola::mostrarBienvenida()
{
    limpiarPantalla();
    std::cout << "=============================================\n"
              << "          BIENVENIDO A SCRABBLE              \n"
              << "=============================================\n"
              << "Tablero 15x15 | 2 a 4 jugadores | Atril de 6\n"
              << "=============================================\n\n";
}

void Consola::mostrarDespedida()
{
    std::cout << "\n=============================================\n"
              << "             GRACIAS POR JUGAR               \n"
              << "=============================================\n";
}
