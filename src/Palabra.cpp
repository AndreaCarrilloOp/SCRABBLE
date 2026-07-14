#include "Palabra.h"
#include <sstream>

Palabra::Palabra() : palabra(), puntaje(0) {}
Palabra::Palabra(const std::string &palabra) : palabra(palabra), puntaje(0) {}
Palabra::Palabra(const std::string &palabra, int puntaje) : palabra(palabra), puntaje(puntaje) {}
void Palabra::setPalabra(const std::string &nuevaPalabra) { palabra = nuevaPalabra; }
std::string Palabra::getPalabra() const { return palabra; }
void Palabra::setPuntaje(int nuevoPuntaje) { puntaje = nuevoPuntaje; }
int Palabra::getPuntaje() const { return puntaje; }
void Palabra::guardar(std::ofstream &archivo) const { archivo << palabra << ' ' << puntaje << '\n'; }
void Palabra::leer(std::ifstream &archivo) { archivo >> palabra >> puntaje; }
std::string Palabra::toString() const
{
    std::ostringstream salida;
    salida << palabra << " (" << puntaje << " puntos)";
    return salida.str();
}
