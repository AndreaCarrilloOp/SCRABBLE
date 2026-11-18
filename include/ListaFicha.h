#ifndef LISTAFICHA_H
#define LISTAFICHA_H

#include "Ficha.h"
#include <string>

class ListaFicha
{
private:
    struct Nodo
    {
        Ficha *ficha;
        Nodo *siguiente;
        explicit Nodo(Ficha *f) : ficha(f), siguiente(nullptr) {}
    };

    Nodo *cabeza;
    Nodo *cola;
    int cantidad;

public:
    ListaFicha();
    ~ListaFicha();

    ListaFicha(const ListaFicha &) = delete;
    ListaFicha &operator=(const ListaFicha &) = delete;

    int getCantidad() const;
    Ficha *getLetra(int indice) const;

    void ingresaFicha(Ficha *ficha);
    bool eliminarFicha(char letra);
    Ficha *eliminarFichaEspecifica(char letra);
    Ficha *eliminarRetornar(int indice);
    Ficha *extraerFicha();
    void barajar();

    bool encuentrada(char letra) const;
    bool seEncuentra(char letra) const;
    int contar(char letra) const;

    void guardarLetras(const std::string &ruta = "fichas.txt") const;
    void cargarLetras(const std::string &ruta = "fichas.txt");

    std::string toString() const;
    std::string toStringJugador() const;
};

#endif
