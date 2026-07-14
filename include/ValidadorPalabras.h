#ifndef VALIDADORPALABRAS_H
#define VALIDADORPALABRAS_H

#include "IDiccionarioPalabras.h"
#include <string>

class ValidadorPalabras
{
private:
    IDiccionarioPalabras *diccionario;

public:
    explicit ValidadorPalabras(IDiccionarioPalabras *diccionario);
    bool palabraValida(const std::string &palabra) const;
    bool palabraCruzaCentro(int fila, int columna, const std::string &palabra, char orientacion) const;
};

#endif
