#include "Jugador.h"

//Constructores y Destructor
Jugador::Jugador() {
	this->nombre = " Jugador ";
	this->fich = new FichaLista();
	this->pal = new  ListaPalabra();
}

Jugador::Jugador(string nombre) {
	this->nombre = nombre;
	letra = new FichaLista();
	palabra = new ListaPalabra();
}

Jugador::~Jugador() {}

//Propeerties
void Jugador::setNombre(string nombre) {
	this->nombre = nombre;
}

string Jugador::getNombre() {
	return nombre;
}

FichaLista* Jugador::getLetra() {
	return letra;
}

ListaPalabra* Jugador::getPalabra() {
	return palabra;
}

//Metodos

//Metodo que retorna la cantidad de fichas, quitando las 7 de cada atril
int Jugador::cantidadFichas() {
	return 6 - letra->getCantidad();// le resta las 6 fichas
}

//Metodo que guarda en el arcivo el nombre y el puntaje del jugador
void Jugador::save(ofstream& file) {
	file << nombre << endl;
}

//Metodo que lee en el archivo el nombre y el puntaje
void Jugador::read(ifstream& file) {
	file >> nombre;
}

//Metodo toString
string Jugador::toString() {
	stringstream s;
	s << this->nombre << " " << endl;
	return s.str();
}

