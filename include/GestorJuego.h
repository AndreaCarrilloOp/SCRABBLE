#ifndef GESTORJUEGO_H
#define GESTORJUEGO_H

#include "IDiccionarioPalabras.h"
#include "ITablero.h"
#include "Tablero.h"

class GestorJuego
{
private:
    ITablero *interfaz;
    Tablero *juego;
    IDiccionarioPalabras *diccionario;

    void mostrarEstado(int jugadorActual) const;
    bool ejecutarJugada(Jugador *jugador);
    bool ejecutarCambio(Jugador *jugador);
    void mostrarResultadosFinales(bool aplicarAjuste);

public:
    GestorJuego();
    ~GestorJuego();

    void inicializarJuego();
    void ejecutarJuego();
};

#endif
