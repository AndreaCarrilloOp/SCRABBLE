#ifndef PALABRA_H
#define PALABRA_H

#include <fstream>
#include <string>

class Palabra
{
private:
    std::string palabra;
    int puntaje;

public:
    Palabra();
    explicit Palabra(const std::string &palabra);
    Palabra(const std::string &palabra, int puntaje);
    virtual ~Palabra() = default;

    void setPalabra(const std::string &palabra);
    std::string getPalabra() const;
    void setPuntaje(int puntaje);
    int getPuntaje() const;

    void guardar(std::ofstream &archivo) const;
    void leer(std::ifstream &archivo);
    std::string toString() const;
};

#endif
