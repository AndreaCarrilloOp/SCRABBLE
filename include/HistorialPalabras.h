#ifndef HISTORIALPALABRAS_H
#define HISTORIALPALABRAS_H

#include <string>
#include <vector>

class HistorialPalabras
{
private:
    std::vector<std::string> palabrasJugadas;

public:
    void agregarPalabra(const std::string &palabra);
    std::vector<std::string> obtenerHistorial() const;
    std::string toString() const;
    void limpiarHistorial();
};

#endif
