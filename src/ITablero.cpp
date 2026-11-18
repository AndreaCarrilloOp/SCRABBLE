#include "ITablero.h"
#include "Consola.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <limits>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#endif

int ITablero::leerEntero(const std::string &mensaje, int minimo, int maximo)
{
    while (true)
    {
        std::cout << mensaje;
        std::string linea;
        if (!std::getline(std::cin, linea))
            return maximo;
        std::istringstream entrada(linea);
        int valor;
        char sobrante;
        if ((entrada >> valor) && !(entrada >> sobrante) && validarEntrada(valor, minimo, maximo))
            return valor;
        std::cout << "Entrada inválida. Ingrese un número entre " << minimo
                  << " y " << maximo << ".\n";
    }
}

int ITablero::menuPrincipal()
{
    std::cout << "\n============== MENÚ ==============\n"
              << "1. Jugar una palabra\n"
              << "2. Cambiar fichas\n"
              << "3. Ver historial de palabras\n"
              << "4. Pasar turno\n"
              << "5. Terminar juego\n"
              << "==================================\n";
    return leerEntero("Seleccione una opción: ", 1, 5);
}

int ITablero::solicitarNumeroJugadores()
{
    return leerEntero("Número de jugadores (2-4): ", 2, 4);
}
int ITablero::solicitarColumna() { return leerEntero("Columna inicial (0-14): ", 0, 14); }
int ITablero::solicitarFila() { return leerEntero("Fila inicial (0-14): ", 0, 14); }

std::string ITablero::solicitarPalabra()
{
    while (true)
    {
        std::cout << "Palabra (2-6 letras, sin tildes): ";
        std::string palabra;
        if (!std::getline(std::cin, palabra))
            return "";
        palabra.erase(std::remove_if(palabra.begin(), palabra.end(), [](unsigned char c) {
            return std::isspace(c) != 0;
        }), palabra.end());
        for (char &c : palabra)
            c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        if (palabra.size() >= 2 && palabra.size() <= 6 && validarPalabra(palabra))
            return palabra;
        std::cout << "La palabra debe contener entre 2 y 6 letras de la A a la Z.\n";
    }
}

char ITablero::solicitarOrientacion()
{
    while (true)
    {
        std::cout << "Orientación H (horizontal) o V (vertical): ";
        std::string entrada;
        if (!std::getline(std::cin, entrada))
            return 'H';
        std::istringstream flujo(entrada);
        char orientacion, sobrante;
        if ((flujo >> orientacion) && !(flujo >> sobrante))
        {
            orientacion = static_cast<char>(std::toupper(static_cast<unsigned char>(orientacion)));
            if (orientacion == 'H' || orientacion == 'V')
                return orientacion;
        }
        std::cout << "Ingrese H o V.\n";
    }
}

std::string ITablero::solicitarLetrasCambio()
{
    std::cout << "Letras que desea cambiar, sin espacios: ";
    std::string letras;
    std::getline(std::cin, letras);
    letras.erase(std::remove_if(letras.begin(), letras.end(), [](unsigned char c) {
        return std::isspace(c) != 0;
    }), letras.end());
    for (char &c : letras)
        c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    return letras;
}

bool ITablero::solicitarConfirmacion(const std::string &mensaje)
{
    while (true)
    {
        std::cout << mensaje << " (S/N): ";
        std::string entrada;
        if (!std::getline(std::cin, entrada))
            return true;
        std::istringstream flujo(entrada);
        char respuesta, sobrante;
        if (!(flujo >> respuesta) || (flujo >> sobrante))
            continue;
        respuesta = static_cast<char>(std::toupper(static_cast<unsigned char>(respuesta)));
        if (respuesta == 'S')
            return true;
        if (respuesta == 'N')
            return false;
    }
}

void ITablero::mostrarBienvenida()
{
    Consola::mostrarBienvenida();
}

void ITablero::mostrarReglas()
{
    std::cout << "\nREGLAS BÁSICAS\n"
              << "- La primera palabra debe cruzar el centro (7,7).\n"
              << "- Las demás deben conectarse con las fichas existentes.\n"
              << "- Todas las palabras y cruces deben estar en el diccionario.\n"
              << "- Las casillas de bono se aplican solo al colocar una ficha nueva.\n";
}

void ITablero::indicadorLeyenda()
{
    std::cout << "** centro | 2P doble palabra | 3P triple palabra | "
              << "2L doble letra | 3L triple letra\n";
}
void ITablero::mostrarPuntajes() { std::cout << "Puntajes actuales:\n"; }
void ITablero::mostrarGanador(const std::string &nombreGanador, int puntaje)
{
    std::cout << "\nGanador: " << nombreGanador << " con " << puntaje << " puntos.\n";
}
void ITablero::limpiarPantalla() { Consola::limpiarPantalla(); }
void ITablero::pausar()
{
    std::cout << "\nPresione Enter para continuar...";
    std::string linea;
    std::getline(std::cin, linea);
}
void ITablero::cambiarColor(int color)
{
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), static_cast<WORD>(color));
#else
    (void)color;
#endif
}
bool ITablero::validarEntrada(int valor, int minimo, int maximo) const
{
    return valor >= minimo && valor <= maximo;
}
bool ITablero::validarLetra(char letra) const
{
    letra = static_cast<char>(std::toupper(static_cast<unsigned char>(letra)));
    return letra >= 'A' && letra <= 'Z';
}
bool ITablero::validarPalabra(const std::string &palabra) const
{
    return !palabra.empty() && std::all_of(palabra.begin(), palabra.end(), [this](char c) {
        return validarLetra(c);
    });
}
