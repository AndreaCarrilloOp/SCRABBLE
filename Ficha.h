#ifndef FICHA_H
#define FICHA_H

#include<fstream>
#include<sstream>

using namespace std;

class Ficha
{
private:
	char letra;
	int cantidad;

public:
	Ficha();
	Ficha(char, int);
	~Ficha();

	virtual void setLetra(char);
	virtual char getLetra();
	virtual void setCantidad(int);
	virtual int getCantidad();

	virtual void guardar(ofstream&);
	virtual void leer(ifstream&);

	virtual string imprimir(); //toString

};

#endif //FICHA_H
