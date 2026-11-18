#ifndef TABLERO_H
#define TABLERO_H

#include "Cuadricula.h"
#include "HistorialPalabras.h"
#include "IDiccionarioPalabras.h"
#include "Jugador.h"
#include "ListaFicha.h"
#include "ListaJugador.h"
#include "ValidadorPalabras.h"
#include <cstddef>
#include <string>
#include <vector>

class Tablero
{
public:
    static constexpr int TAMANIO = 15;
    static constexpr int MAX_FICHAS_ATRIL = 6;
    static constexpr int MAX_LONGITUD_PALABRA = 6;

private:
    Cuadricula *tabla[TAMANIO][TAMANIO];
    ListaJugador *jugadores;
    ListaFicha *bolsa;
    int numJugadores;
    IDiccionarioPalabras *diccionario; // No propietario.
    ValidadorPalabras *validador;
    HistorialPalabras *historial;

    static std::string normalizarPalabra(const std::string &palabra);
    bool dentroDelTablero(int fila, int columna) const;
    bool tableroVacio() const;
    bool coordenadasValidas(int fila, int columna, std::size_t longitud, char orientacion) const;
    bool validarJugada(int fila, int columna, const std::string &palabra,
                        char orientacion, Jugador *jugador, std::string &palabraCompleta,
                        std::string *error) const;
    bool tieneConexion(int fila, int columna, const std::string &palabra, char orientacion) const;
    bool jugadorTieneFichas(const Jugador *jugador, int fila, int columna,
                            const std::string &palabra, char orientacion) const;
    std::string construirCruce(int fila, int columna, char letra, char orientacion) const;
    int calcularPuntajePalabra(int fila, int columna, const std::string &palabra,
                               char orientacion) const;
    int calcularPuntajeCruces(int fila, int columna, const std::string &palabra,
                              char orientacion) const;
    void reponerAtril(Jugador *jugador);

public:
    Tablero();
    explicit Tablero(IDiccionarioPalabras *dict);
    ~Tablero();

    Tablero(const Tablero &) = delete;
    Tablero &operator=(const Tablero &) = delete;

    void setNumJugadores(int numJugadores);
    int getNumJugadores() const;
    void setJugadoresLista(ListaJugador *lista);
    ListaJugador *getJugadores() const;
    Cuadricula *getCuadricula(int fila, int columna) const;
    ListaFicha *getLista() const;
    void setValidador(ValidadorPalabras *validador);
    ValidadorPalabras *getValidador() const;
    void setHistorial(HistorialPalabras *historial);
    HistorialPalabras *getHistorial() const;

    void llenarBolsa();
    void agregarJugador(int cantidad);
    void inicializarTablero();
    void repartirFichas(Jugador *jugador, int cantidad);
    int jugadorInicial() const;
    bool vacio() const;

    void palabraInicial(std::string palabra, char posicion, Jugador *jugador);
    bool palabraAprobada(int columna, int fila, Jugador *jugador,
                         std::string palabra, char orientacion = 'H');
    bool compararArchivo(std::string palabra) const;
    bool colocarPalabra(int x, int y, std::string palabra, char letra,
                        char posicion, Jugador *jugador);
    void acomodarPalabraInicial(std::string palabra, char posicion, Jugador *jugador);
    bool acomodarPalabra(int fila, int columna, std::string palabra,
                         char letra, char posicion, Jugador *jugador);

    bool palabraCruzaCentro(int fila, int columna, const std::string &palabra,
                            char orientacion) const;
    std::string construirPalabraExtendida(int fila, int columna,
                                          const std::string &palabra,
                                          char orientacion) const;
    bool validarCruces(int fila, int columna, const std::string &palabra,
                       char orientacion) const;

    void cambiarFichas(Jugador *jugador, const std::string &letras);
    bool intercambiarFichas(Jugador *jugador, const std::string &letras,
                            std::string *error = nullptr);
    void colocarPalabraEnTablero(int fila, int columna, const std::string &palabra,
                                 char orientacion, Jugador *jugador);
    bool jugarPalabra(int fila, int columna, const std::string &palabra,
                      char orientacion, Jugador *jugador, std::string *error = nullptr,
                      int *puntajeObtenido = nullptr);
    void registrarPalabraJugada(const std::string &jugador,
                                const std::string &palabra, int puntaje = 0);
    std::string obtenerHistorialPalabras() const;

    char buscarLetra(int columna, int fila) const;
    char posicion(int columna, int fila) const;
    char extraerLetra(int fila, int columna) const;
    int determinarPuntaje(Jugador *jugador, Ficha *ficha, char letra,
                          bool especial, std::string bono) const;

    bool ganador(int pasar, bool sinLetras);
    bool hayGanador(int pasar = 0, bool sinLetras = false);
    void aplicarPuntajeFinal();
    void juegoCompleto();

    void mostrar() const;
    void mostrar(bool colores) const;
    std::string toString() const;
    std::string toString(bool colores) const;
    std::string toStringJugador() const;
};

#endif
