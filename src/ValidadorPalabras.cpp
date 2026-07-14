#include "ValidadorPalabras.h"
#include <cctype>

ValidadorPalabras::ValidadorPalabras(IDiccionarioPalabras *diccionario)
    : diccionario(diccionario)
{
}
bool ValidadorPalabras::palabraValida(const std::string &palabra) const
{
    return diccionario && diccionario->contiene(palabra);
}
bool ValidadorPalabras::palabraCruzaCentro(int fila, int columna,
                                           const std::string &palabra,
                                           char orientacion) const
{
    orientacion = static_cast<char>(std::toupper(static_cast<unsigned char>(orientacion)));
    const int fin = static_cast<int>(palabra.size()) - 1;
    if (orientacion == 'H')
        return fila == 7 && columna <= 7 && columna + fin >= 7;
    if (orientacion == 'V')
        return columna == 7 && fila <= 7 && fila + fin >= 7;
    return false;
}
