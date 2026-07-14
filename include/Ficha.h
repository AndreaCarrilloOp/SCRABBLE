#ifndef FICHA_H
#define FICHA_H

#include <fstream>
#include <sstream>
#include <string>

class Ficha
{
private:
    char letra;
    int cantidad;

public:
    Ficha();
    Ficha(char letra, int cantidad = 1);
    virtual ~Ficha() = default;

    void setLetra(char letra);
    char getLetra() const;
    void setCantidad(int cantidad);
    int getCantidad() const;
    int getPuntaje() const;

    void guardar(std::ofstream &archivo) const;
    void leer(std::ifstream &archivo);
    std::string imprimir() const;

    bool esLetraCompartida(char otraLetra) const;
};

#endif
