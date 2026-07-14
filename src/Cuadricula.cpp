#include "Cuadricula.h"

Cuadricula::Cuadricula()
    : letra(nullptr), ocupada(false), bono("Normal"),
      multiplicadorLetra(1), multiplicadorPalabra(1)
{
}
Cuadricula::Cuadricula(const std::string &bono, int multLetra, int multPalabra)
    : letra(nullptr), ocupada(false), bono(bono),
      multiplicadorLetra(multLetra), multiplicadorPalabra(multPalabra)
{
}
Cuadricula::~Cuadricula() { delete letra; }
Ficha *Cuadricula::getLetra() const { return letra; }
void Cuadricula::setLetra(Ficha *ficha)
{
    if (letra != ficha)
    {
        delete letra;
        letra = ficha;
    }
    ocupada = (letra != nullptr);
}
bool Cuadricula::isOcupada() const { return ocupada; }
void Cuadricula::setOcupada(bool estado) { ocupada = estado; }
std::string Cuadricula::getBono() const { return bono; }
void Cuadricula::setBono(const std::string &nuevoBono) { bono = nuevoBono; }
int Cuadricula::getMultiplicadorLetra() const { return multiplicadorLetra; }
void Cuadricula::setMultiplicadorLetra(int multiplicador) { multiplicadorLetra = multiplicador; }
int Cuadricula::getMultiplicadorPalabra() const { return multiplicadorPalabra; }
void Cuadricula::setMultiplicadorPalabra(int multiplicador) { multiplicadorPalabra = multiplicador; }
void Cuadricula::limpiarCuadricula()
{
    delete letra;
    letra = nullptr;
    ocupada = false;
}
std::string Cuadricula::toString() const
{
    return ocupada && letra ? std::string(1, letra->getLetra()) : " ";
}
