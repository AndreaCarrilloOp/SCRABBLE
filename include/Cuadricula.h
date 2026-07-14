#ifndef CUADRICULA_H
#define CUADRICULA_H

#include "Ficha.h"
#include <string>

class Cuadricula
{
private:
    Ficha *letra;
    bool ocupada;
    std::string bono;
    int multiplicadorLetra;
    int multiplicadorPalabra;

public:
    Cuadricula();
    Cuadricula(const std::string &bono, int multLetra, int multPalabra);
    ~Cuadricula();

    Ficha *getLetra() const;
    void setLetra(Ficha *ficha);
    bool isOcupada() const;
    void setOcupada(bool estado);
    std::string getBono() const;
    void setBono(const std::string &bono);
    int getMultiplicadorLetra() const;
    void setMultiplicadorLetra(int multiplicador);
    int getMultiplicadorPalabra() const;
    void setMultiplicadorPalabra(int multiplicador);

    void limpiarCuadricula();
    std::string toString() const;
};

#endif
