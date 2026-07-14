#ifndef IDICCIONARIO_PALABRAS_H
#define IDICCIONARIO_PALABRAS_H

#include <string>

class IDiccionarioPalabras
{
public:
    virtual ~IDiccionarioPalabras() = default;
    virtual bool contiene(const std::string &palabra) const = 0;
};

#endif
