#include "GestorJuego.h"
#include "Consola.h"
#include "FabricaDiccionarioPalabras.h"
#include <filesystem>
#include <iostream>
#include <stdexcept>

namespace
{
std::string localizarDiccionario()
{
    const std::string candidatos[] = {
        "Datos.txt", "../Datos.txt", "../../Datos.txt", "../../../Datos.txt"
    };
    for (const std::string &ruta : candidatos)
        if (std::filesystem::exists(ruta))
            return ruta;
    throw std::runtime_error("No se encontró Datos.txt. Colóquelo junto al ejecutable o en la carpeta del proyecto.");
}
}

GestorJuego::GestorJuego() : interfaz(nullptr), juego(nullptr), diccionario(nullptr) {}

GestorJuego::~GestorJuego()
{
    delete juego;
    delete interfaz;
    delete diccionario;
}

void GestorJuego::inicializarJuego()
{
    Consola::configurarConsola();
    Consola::mostrarBienvenida();

    interfaz = new ITablero();
    diccionario = CrearDiccionarioDesdeArchivo(localizarDiccionario());
    juego = new Tablero(diccionario);

    interfaz->mostrarReglas();
    int cantidad = interfaz->solicitarNumeroJugadores();
    if (!std::cin)
        throw std::runtime_error("Registro de jugadores interrumpido.");
    juego->setNumJugadores(cantidad);
    juego->llenarBolsa();
    juego->agregarJugador(cantidad);
    for (int i = 0; i < juego->getJugadores()->getCantidad(); ++i)
        juego->repartirFichas(juego->getJugadores()->getJugador(i), Tablero::MAX_FICHAS_ATRIL);
}

void GestorJuego::mostrarEstado(int jugadorActual) const
{
    interfaz->limpiarPantalla();
    juego->mostrar();
    std::cout << "\n============== MARCADOR ==============\n";
    for (int i = 0; i < juego->getJugadores()->getCantidad(); ++i)
    {
        Jugador *jugador = juego->getJugadores()->getJugador(i);
        std::cout << (i == jugadorActual ? "> " : "  ")
                  << jugador->getNombre() << ": " << jugador->getPuntaje() << " puntos\n";
    }
    Jugador *activo = juego->getJugadores()->getJugador(jugadorActual);
    std::cout << "======================================\n"
              << "Turno de " << activo->getNombre() << '\n'
              << activo->getFicha()->toStringJugador() << '\n'
              << "Fichas restantes en la bolsa: " << juego->getLista()->getCantidad() << "\n";
}

bool GestorJuego::ejecutarJugada(Jugador *jugador)
{
    std::string palabra = interfaz->solicitarPalabra();
    char orientacion = interfaz->solicitarOrientacion();
    int fila = interfaz->solicitarFila();
    int columna = interfaz->solicitarColumna();
    if (!std::cin)
        return false;

    std::string error;
    int puntos = 0;
    if (!juego->jugarPalabra(fila, columna, palabra, orientacion, jugador, &error, &puntos))
    {
        std::cout << "\nJugada rechazada: " << error << '\n';
        interfaz->pausar();
        return false;
    }
    std::cout << "\nPalabra aceptada. Obtuvo " << puntos << " puntos.\n";
    interfaz->pausar();
    return true;
}

bool GestorJuego::ejecutarCambio(Jugador *jugador)
{
    std::cout << jugador->getFicha()->toStringJugador() << '\n';
    std::string letras = interfaz->solicitarLetrasCambio();
    if (!std::cin)
        return false;
    std::string error;
    if (!juego->intercambiarFichas(jugador, letras, &error))
    {
        std::cout << "\nNo se realizó el cambio: " << error << '\n';
        interfaz->pausar();
        return false;
    }
    std::cout << "\nCambio realizado. Nuevo " << jugador->getFicha()->toStringJugador() << '\n';
    interfaz->pausar();
    return true;
}

void GestorJuego::mostrarResultadosFinales(bool aplicarAjuste)
{
    if (aplicarAjuste)
        juego->aplicarPuntajeFinal();
    juego->getJugadores()->ordenarPorPuntaje();
    interfaz->limpiarPantalla();
    std::cout << "============== RESULTADOS FINALES ==============\n"
              << juego->getJugadores()->toStringJugador()
              << "=================================================\n";
    Jugador *ganador = juego->getJugadores()->getJugador(0);
    if (ganador)
        interfaz->mostrarGanador(ganador->getNombre(), ganador->getPuntaje());
}

void GestorJuego::ejecutarJuego()
{
    if (!juego || juego->getJugadores()->getCantidad() == 0)
        throw std::runtime_error("El juego no fue inicializado correctamente.");

    int jugadorActual = juego->jugadorInicial();
    int pasesConsecutivos = 0;
    bool terminar = false;
    bool finNatural = false;

    while (!terminar && std::cin)
    {
        mostrarEstado(jugadorActual);
        Jugador *activo = juego->getJugadores()->getJugador(jugadorActual);
        bool consumirTurno = false;

        switch (interfaz->menuPrincipal())
        {
        case 1:
            consumirTurno = ejecutarJugada(activo);
            if (consumirTurno)
                pasesConsecutivos = 0;
            break;
        case 2:
            consumirTurno = ejecutarCambio(activo);
            if (consumirTurno)
                pasesConsecutivos = 0;
            break;
        case 3:
            std::cout << "\n============== HISTORIAL ==============\n"
                      << juego->obtenerHistorialPalabras()
                      << "=======================================\n";
            interfaz->pausar();
            break;
        case 4:
            ++pasesConsecutivos;
            consumirTurno = true;
            std::cout << activo->getNombre() << " pasó el turno.\n";
            interfaz->pausar();
            break;
        case 5:
            terminar = interfaz->solicitarConfirmacion("¿Desea terminar la partida?");
            break;
        }

        if (consumirTurno)
        {
            if (juego->hayGanador(pasesConsecutivos, false))
            {
                terminar = true;
                finNatural = true;
            }
            else
                jugadorActual = (jugadorActual + 1) % juego->getJugadores()->getCantidad();
        }
    }

    mostrarResultadosFinales(finNatural);
    Consola::mostrarDespedida();
}
