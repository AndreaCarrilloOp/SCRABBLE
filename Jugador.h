#pragma once
#include"FichaLista.h"
#include"ListaPalabra.h"
#include<fstream>
#include<string>
#include<sstream>
using namespace std;

class Jugador {
private:
	string nombre;
	FichaLista* letra;//cambiar nombre
	PalabraLista* palabra;//cambiarnombre
public:
	//Constructores y Destructor
	Jugador();
	Jugador(string);
	~Jugador();

	//Properties
	void setNombre(string);
	string getNombre();
	FichaLista* getLetra();
	PalabraLista* getPalabra();

	//Metodos
	int cantidadFichas();
	void save(ofstream&);
	void read(ifstream&);
	string toString();
};//end class
