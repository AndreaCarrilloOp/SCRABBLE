#ifndef LISTAJUGADOR_H
#define LISTAJUGADOR_H

#include "Jugador.h"
#include <string>

class ListaJugador
{
public:
    struct Nodo
    {
        Jugador *jugador;
        Nodo *siguiente;
        explicit Nodo(Jugador *j) : jugador(j), siguiente(nullptr) {}
    };

private:
    Nodo *cabeza;
    int cantidad;

    Nodo *mergeSort(Nodo *head);
    Nodo *merge(Nodo *left, Nodo *right);
    Nodo *getMiddle(Nodo *head);

public:
    ListaJugador();
    ~ListaJugador();

    ListaJugador(const ListaJugador &) = delete;
    ListaJugador &operator=(const ListaJugador &) = delete;

    int getCantidad() const;
    Jugador *getJugador(int indice) const;
    void ingresaJugadorInicio(Jugador *jugador);
    void ingresaJugadorFinal(Jugador *jugador);
    bool eliminarJugador(const std::string &nombre);
    Jugador *buscarJugador(const std::string &nombre) const;
    int obtenerIndiceJugador(const std::string &nombre) const;
    void ordenarPorPuntaje();
    void guardarJugadores(const std::string &ruta = "jugadores.txt") const;
    void cargarJugadores(const std::string &ruta = "jugadores.txt");
    std::string toString() const;
    std::string toStringJugador() const;
};

#endif
