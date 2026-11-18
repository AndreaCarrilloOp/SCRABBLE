#include "Consola.h"
#include <cstdlib>
#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

bool Consola::soportaColor()
{
    if (std::getenv("NO_COLOR"))
        return false;
#ifdef _WIN32
    const HANDLE salida = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD modo = 0;
    return salida != INVALID_HANDLE_VALUE && GetConsoleMode(salida, &modo) &&
           SetConsoleMode(salida, modo | ENABLE_PROCESSED_OUTPUT |
                          ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#else
    const char *term = std::getenv("TERM");
    return isatty(STDOUT_FILENO) != 0 && (!term || std::string(term) != "dumb");
#endif
}

void Consola::configurarConsola()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    SetConsoleTitleA("SCRABBLE - Juego en C++");
#endif
    (void)soportaColor();
}

void Consola::limpiarPantalla()
{
    if (soportaColor())
        std::cout << "\033[2J\033[H";
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
