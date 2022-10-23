#include "Ficha.h"

Ficha::Ficha() {
	letra = ' ';
	cantidad = 0;
}

Ficha::Ficha(char letra, int cantidad) {
	this->letra = letra;
	this->cantidad = cantidad;
}

Ficha::~Ficha() {}

void Ficha::setLetra(char letra) {
	this->letra = letra;
}

char Ficha::getLetra() {
	return this->letra;
}

void Ficha::setCantidad(int cantidad) {
	this->cantidad = cantidad;
}

int Ficha::getCantidad() {
	return this->cantidad;
}

void Ficha::guardar(ofstream& file) {
	file << letra << endl << cantidad << endl;
}

void Ficha::leer(ifstream& file) {
	file >> letra >> cantidad;
}

string Ficha::imprimir() { //toString
	stringstream s;
	s << " Letra: " << letra << " Cantidad: " << cantidad << endl;
	return s.str();
}
