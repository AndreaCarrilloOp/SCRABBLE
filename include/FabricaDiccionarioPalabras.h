#ifndef FABRICA_DICCIONARIO_PALABRAS_H
#define FABRICA_DICCIONARIO_PALABRAS_H

#include "IDiccionarioPalabras.h"
#include <string>

IDiccionarioPalabras *CrearDiccionarioDesdeArchivo(const std::string &ruta);

#endif
