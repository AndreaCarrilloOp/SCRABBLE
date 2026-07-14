#ifndef ITABLERO_H
#define ITABLERO_H

#include <string>

class ITablero
{
private:
    int leerEntero(const std::string &mensaje, int minimo, int maximo);

public:
    ITablero() = default;
    ~ITablero() = default;

    int menuPrincipal();
    int solicitarNumeroJugadores();
    int solicitarColumna();
    int solicitarFila();
    std::string solicitarPalabra();
    char solicitarOrientacion();
    std::string solicitarLetrasCambio();
    bool solicitarConfirmacion(const std::string &mensaje);

    void mostrarBienvenida();
    void mostrarReglas();
    void indicadorLeyenda();
    void mostrarPuntajes();
    void mostrarGanador(const std::string &nombreGanador, int puntaje);

    void limpiarPantalla();
    void pausar();
    void cambiarColor(int color);

    bool validarEntrada(int valor, int minimo, int maximo) const;
    bool validarLetra(char letra) const;
    bool validarPalabra(const std::string &palabra) const;
};

#endif
