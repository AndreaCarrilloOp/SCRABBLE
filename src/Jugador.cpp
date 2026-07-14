#include "Jugador.h"
#include <sstream>

Jugador::Jugador() : Jugador("Jugador", 0) {}
Jugador::Jugador(const std::string &nombre) : Jugador(nombre, 0) {}

Jugador::Jugador(const std::string &nombre, int puntaje)
    : nombre(nombre.empty() ? "Jugador" : nombre), puntaje(puntaje),
      fichas(new ListaFicha()), palabras(new ListaPalabra())
{
}

Jugador::~Jugador()
{
    delete fichas;
    delete palabras;
}

void Jugador::setNombre(const std::string &nuevoNombre)
{
    if (!nuevoNombre.empty())
        nombre = nuevoNombre;
}

std::string Jugador::getNombre() const { return nombre; }
ListaFicha *Jugador::getLetra() { return fichas; }
ListaFicha *Jugador::getFicha() { return fichas; }
ListaFicha *Jugador::getFich() { return fichas; }
ListaPalabra *Jugador::getPalabra() { return palabras; }
ListaPalabra *Jugador::getPal() { return palabras; }
int Jugador::getPuntaje() const { return puntaje; }
void Jugador::setPuntaje(int nuevoPuntaje) { puntaje = nuevoPuntaje; }
void Jugador::incrementarPuntaje(int puntos) { puntaje += puntos; }

int Jugador::cantidadFichas() const
{
    return std::max(0, 6 - fichas->getCantidad());
}

void Jugador::agregarFicha(Ficha *ficha) { fichas->ingresaFicha(ficha); }
Ficha *Jugador::quitarFicha(char letra) { return fichas->eliminarFichaEspecifica(letra); }
bool Jugador::tieneFicha(char letra) const { return fichas->encuentrada(letra); }

void Jugador::reemplazarFichas(const std::string &letras)
{
    for (char letra : letras)
        fichas->eliminarFicha(letra);
}

void Jugador::save(std::ofstream &archivo) const
{
    archivo << nombre << '\n' << puntaje << '\n';
}

void Jugador::read(std::ifstream &archivo)
{
    std::getline(archivo >> std::ws, nombre);
    archivo >> puntaje;
}

std::string Jugador::toString() const
{
    std::ostringstream salida;
    salida << nombre << " - " << puntaje << " puntos";
    return salida.str();
}
