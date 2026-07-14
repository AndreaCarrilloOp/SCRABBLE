#ifndef LISTAPALABRA_H
#define LISTAPALABRA_H

#include "Palabra.h"
#include <string>

class ListaPalabra
{
private:
    struct Nodo
    {
        Palabra *palabra;
        Nodo *siguiente;
        explicit Nodo(Palabra *p) : palabra(p), siguiente(nullptr) {}
    };

    Nodo *cabeza;
    int cantidad;

public:
    ListaPalabra();
    ~ListaPalabra();

    int getCantidad() const;
    Palabra *getPalabra(int indice) const;
    void ingresaPalabra(Palabra *palabra);
    bool eliminarPalabra(const std::string &palabra);
    bool buscarPalabra(const std::string &palabra) const;

    void guardarPalabras(int jugadorId) const;
    void cargarPalabras(int jugadorId);
    std::string toString() const;
    std::string toStringJugador() const;

    void manejarLetrasCompartidas(Palabra *nuevaPalabra);
    bool verificarPalabraEnBaseDeDatos(const std::string &palabra) const;
};

#endif
