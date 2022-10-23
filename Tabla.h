#ifndef TABLA_H
#define TABLA_H

#include<iostream> 
#include <string>
#include <cstdlib>
#include <Windows.h>
#include <time.h>
#include "Cuadricula.h"
#include "JugadorLista.h"
#include "InterfazTabla.h"

using namespace std;

class Tabla
{
private:
	Cuadricula* tabla[15][15];
	int jugadores;
	JugadorLista* jugador;
	FichaLista* letra;

public:
	Tabla();
	~Tabla();


	virtual void setNumJugadores(int);
	virtual int getNumJugadores();
	virtual void setJugadoresLista(ListaJugador*);
	virtual JugadorLista* getJugadores();
	virtual Cuadricula* getCuadricula(int, int);
	virtual ListaFicha* getLista();

	void llenarBolsa();
	void agregarJugador(int);
	| virtual void repartirFichas(Jugador*, int);
	virtual int jugadorInicial();

	bool tablaVacia();//Nos dice si la casilla inicial (7,7) se encuentra ocupado. Osea el tablero esta limpio.
	void palabraInicial(string, char, Jugador*);

	char buscarLetra(int, int);
	bool compararArchivo(string);
	bool palabraAprobada(int, int, Jugador*, string);
	char posicion(int, int); //Determina si es vertical o horizontal.
	bool colocaPalabra(int, int, string, char, char, Jugador*);

	bool ganador(int, bool);
	virtual void juegoCompleto();

	virtual string toStringJugador();
	virtual void mostrar();
};

#endif //TABLA_H TABLA_H