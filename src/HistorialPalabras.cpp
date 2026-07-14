#include "HistorialPalabras.h"
#include <sstream>

void HistorialPalabras::agregarPalabra(const std::string &palabra)
{
    if (!palabra.empty())
        palabrasJugadas.push_back(palabra);
}
std::vector<std::string> HistorialPalabras::obtenerHistorial() const { return palabrasJugadas; }
std::string HistorialPalabras::toString() const
{
    if (palabrasJugadas.empty())
        return "No se han jugado palabras todavía.";
    std::ostringstream salida;
    for (std::size_t i = 0; i < palabrasJugadas.size(); ++i)
        salida << (i + 1) << ". " << palabrasJugadas[i] << '\n';
    return salida.str();
}
void HistorialPalabras::limpiarHistorial() { palabrasJugadas.clear(); }
