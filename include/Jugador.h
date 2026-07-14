#ifndef JUGADOR_H
#define JUGADOR_H

#include "ListaFicha.h"
#include "ListaPalabra.h"
#include <fstream>
#include <string>

class Jugador
{
private:
    std::string nombre;
    int puntaje;
    ListaFicha *fichas;
    ListaPalabra *palabras;

public:
    Jugador();
    explicit Jugador(const std::string &nombre);
    Jugador(const std::string &nombre, int puntaje);
    ~Jugador();

    Jugador(const Jugador &) = delete;
    Jugador &operator=(const Jugador &) = delete;

    void setNombre(const std::string &nombre);
    std::string getNombre() const;

    ListaFicha *getLetra();
    ListaFicha *getFicha();
    ListaFicha *getFich();
    ListaPalabra *getPalabra();
    ListaPalabra *getPal();

    int getPuntaje() const;
    void setPuntaje(int puntaje);
    void incrementarPuntaje(int puntos);

    int cantidadFichas() const;
    void agregarFicha(Ficha *ficha);
    Ficha *quitarFicha(char letra);
    bool tieneFicha(char letra) const;

    void reemplazarFichas(const std::string &letras);
    void save(std::ofstream &archivo) const;
    void read(std::ifstream &archivo);
    std::string toString() const;
};

#endif
