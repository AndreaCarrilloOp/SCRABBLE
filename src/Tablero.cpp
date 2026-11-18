#include "Tablero.h"
#include "Consola.h"
#include <algorithm>
#include <array>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>

namespace
{
char mayuscula(char c)
{
    return static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
}

std::string codigoBono(const Cuadricula *casilla)
{
    if (!casilla || casilla->isOcupada())
        return " ";
    if (casilla->getMultiplicadorPalabra() == 3)
        return "3P";
    if (casilla->getMultiplicadorPalabra() == 2)
        return casilla->getBono() == "Centro" ? "**" : "2P";
    if (casilla->getMultiplicadorLetra() == 3)
        return "3L";
    if (casilla->getMultiplicadorLetra() == 2)
        return "2L";
    return " ";
}

constexpr const char *RESET = "\033[0m";
constexpr const char *VERDE = "\033[38;2;38;150;55m";
constexpr const char *ROJO = "\033[48;2;195;28;40m";
constexpr const char *MAGENTA = "\033[48;2;151;20;144m";
constexpr const char *AZUL = "\033[48;2;43;107;220m";
constexpr const char *CELESTE = "\033[48;2;105;207;210m";
constexpr const char *AMARILLO = "\033[48;2;245;221;112m";
constexpr const char *FICHA = "\033[30;48;2;242;220;176m";

const char *colorBono(const std::string &codigo)
{
    if (codigo == "3P") return ROJO;
    if (codigo == "2P") return MAGENTA;
    if (codigo == "3L") return AZUL;
    if (codigo == "2L") return CELESTE;
    if (codigo == "**") return AMARILLO;
    return nullptr;
}
}

Tablero::Tablero()
    : jugadores(new ListaJugador()), bolsa(new ListaFicha()), numJugadores(0),
      diccionario(nullptr), validador(nullptr), historial(new HistorialPalabras())
{
    for (int fila = 0; fila < TAMANIO; ++fila)
        for (int columna = 0; columna < TAMANIO; ++columna)
            tabla[fila][columna] = new Cuadricula();
    inicializarTablero();
}

Tablero::Tablero(IDiccionarioPalabras *dict) : Tablero()
{
    diccionario = dict;
    validador = new ValidadorPalabras(dict);
}

Tablero::~Tablero()
{
    for (int fila = 0; fila < TAMANIO; ++fila)
        for (int columna = 0; columna < TAMANIO; ++columna)
            delete tabla[fila][columna];
    delete jugadores;
    delete bolsa;
    delete validador;
    delete historial;
}

std::string Tablero::normalizarPalabra(const std::string &palabra)
{
    std::string resultado;
    resultado.reserve(palabra.size());
    for (unsigned char c : palabra)
    {
        if (!std::isspace(c))
            resultado.push_back(static_cast<char>(std::toupper(c)));
    }
    return resultado;
}

bool Tablero::dentroDelTablero(int fila, int columna) const
{
    return fila >= 0 && fila < TAMANIO && columna >= 0 && columna < TAMANIO;
}

bool Tablero::coordenadasValidas(int fila, int columna, std::size_t longitud,
                                  char orientacion) const
{
    if (!dentroDelTablero(fila, columna) || longitud == 0 || longitud > TAMANIO ||
        (orientacion != 'H' && orientacion != 'V'))
        return false;
    const int desplazamiento = static_cast<int>(longitud) - 1;
    return dentroDelTablero(fila + (orientacion == 'V' ? desplazamiento : 0),
                            columna + (orientacion == 'H' ? desplazamiento : 0));
}

bool Tablero::tableroVacio() const
{
    for (int fila = 0; fila < TAMANIO; ++fila)
        for (int columna = 0; columna < TAMANIO; ++columna)
            if (tabla[fila][columna]->isOcupada())
                return false;
    return true;
}

void Tablero::setNumJugadores(int cantidad) { numJugadores = cantidad; }
int Tablero::getNumJugadores() const { return numJugadores; }

void Tablero::setJugadoresLista(ListaJugador *lista)
{
    if (lista && lista != jugadores)
    {
        delete jugadores;
        jugadores = lista;
        numJugadores = lista->getCantidad();
    }
}

ListaJugador *Tablero::getJugadores() const { return jugadores; }
Cuadricula *Tablero::getCuadricula(int fila, int columna) const
{
    return dentroDelTablero(fila, columna) ? tabla[fila][columna] : nullptr;
}
ListaFicha *Tablero::getLista() const { return bolsa; }

void Tablero::setValidador(ValidadorPalabras *nuevoValidador)
{
    if (nuevoValidador != validador)
    {
        delete validador;
        validador = nuevoValidador;
    }
}
ValidadorPalabras *Tablero::getValidador() const { return validador; }

void Tablero::setHistorial(HistorialPalabras *nuevoHistorial)
{
    if (nuevoHistorial && nuevoHistorial != historial)
    {
        delete historial;
        historial = nuevoHistorial;
    }
}
HistorialPalabras *Tablero::getHistorial() const { return historial; }

void Tablero::llenarBolsa()
{
    delete bolsa;
    bolsa = new ListaFicha();

    const std::array<std::pair<char, int>, 26> distribucion = {{
        {'A', 12}, {'B', 2}, {'C', 4}, {'D', 5}, {'E', 12}, {'F', 1},
        {'G', 2}, {'H', 2}, {'I', 6}, {'J', 1}, {'K', 0}, {'L', 4},
        {'M', 2}, {'N', 5}, {'O', 9}, {'P', 2}, {'Q', 1}, {'R', 5},
        {'S', 6}, {'T', 4}, {'U', 5}, {'V', 1}, {'W', 0}, {'X', 1},
        {'Y', 1}, {'Z', 1}
    }};

    for (const auto &[letra, cantidad] : distribucion)
        for (int i = 0; i < cantidad; ++i)
            bolsa->ingresaFicha(new Ficha(letra, 1));
    bolsa->barajar();
}

void Tablero::agregarJugador(int cantidad)
{
    cantidad = std::clamp(cantidad, 2, 4);
    numJugadores = cantidad;
    for (int i = 1; i <= cantidad; ++i)
    {
        std::string nombre;
        std::cout << "Nombre del jugador " << i << ": ";
        if (!std::getline(std::cin, nombre))
            throw std::runtime_error("Registro de jugadores interrumpido.");
        if (nombre.empty())
            nombre = "Jugador " + std::to_string(i);
        std::string base = nombre;
        int sufijo = 2;
        while (jugadores->buscarJugador(nombre))
            nombre = base + " " + std::to_string(sufijo++);
        jugadores->ingresaJugadorFinal(new Jugador(nombre));
    }
}

void Tablero::inicializarTablero()
{
    for (int fila = 0; fila < TAMANIO; ++fila)
    {
        for (int columna = 0; columna < TAMANIO; ++columna)
        {
            Cuadricula *casilla = tabla[fila][columna];
            casilla->setBono("Normal");
            casilla->setMultiplicadorLetra(1);
            casilla->setMultiplicadorPalabra(1);
        }
    }

    const int triplePalabra[][2] = {{0,0},{0,7},{0,14},{7,0},{7,14},{14,0},{14,7},{14,14}};
    const int doblePalabra[][2] = {
        {1,1},{2,2},{3,3},{4,4},{7,7},{10,10},{11,11},{12,12},{13,13},
        {1,13},{2,12},{3,11},{4,10},{10,4},{11,3},{12,2},{13,1}
    };
    const int tripleLetra[][2] = {
        {1,5},{1,9},{5,1},{5,5},{5,9},{5,13},
        {9,1},{9,5},{9,9},{9,13},{13,5},{13,9}
    };
    const int dobleLetra[][2] = {
        {0,3},{0,11},{2,6},{2,8},{3,0},{3,7},{3,14},
        {6,2},{6,6},{6,8},{6,12},{7,3},{7,11},
        {8,2},{8,6},{8,8},{8,12},{11,0},{11,7},{11,14},
        {12,6},{12,8},{14,3},{14,11}
    };

    for (const auto &p : triplePalabra)
    {
        tabla[p[0]][p[1]]->setBono("Triple palabra");
        tabla[p[0]][p[1]]->setMultiplicadorPalabra(3);
    }
    for (const auto &p : doblePalabra)
    {
        tabla[p[0]][p[1]]->setBono(p[0] == 7 && p[1] == 7 ? "Centro" : "Doble palabra");
        tabla[p[0]][p[1]]->setMultiplicadorPalabra(2);
    }
    for (const auto &p : tripleLetra)
    {
        tabla[p[0]][p[1]]->setBono("Triple letra");
        tabla[p[0]][p[1]]->setMultiplicadorLetra(3);
    }
    for (const auto &p : dobleLetra)
    {
        tabla[p[0]][p[1]]->setBono("Doble letra");
        tabla[p[0]][p[1]]->setMultiplicadorLetra(2);
    }
}

void Tablero::repartirFichas(Jugador *jugador, int cantidad)
{
    if (!jugador || cantidad <= 0)
        return;
    for (int i = 0; i < cantidad && bolsa->getCantidad() > 0; ++i)
        jugador->agregarFicha(bolsa->extraerFicha());
}

void Tablero::reponerAtril(Jugador *jugador)
{
    if (jugador)
        repartirFichas(jugador, jugador->cantidadFichas());
}

int Tablero::jugadorInicial() const
{
    if (!jugadores || jugadores->getCantidad() == 0)
        return 0;
    static std::mt19937 generador(std::random_device{}());
    std::uniform_int_distribution<int> distribucion(0, jugadores->getCantidad() - 1);
    return distribucion(generador);
}

bool Tablero::vacio() const { return tableroVacio(); }

bool Tablero::palabraCruzaCentro(int fila, int columna, const std::string &palabra,
                                 char orientacion) const
{
    orientacion = mayuscula(orientacion);
    if (!coordenadasValidas(fila, columna, palabra.size(), orientacion))
        return false;
    const int fin = static_cast<int>(palabra.size()) - 1;
    if (orientacion == 'H')
        return fila == 7 && columna <= 7 && columna + fin >= 7;
    if (orientacion == 'V')
        return columna == 7 && fila <= 7 && fila + fin >= 7;
    return false;
}

bool Tablero::tieneConexion(int fila, int columna, const std::string &palabra,
                            char orientacion) const
{
    if (tableroVacio())
        return palabraCruzaCentro(fila, columna, palabra, orientacion);

    for (int i = 0; i < static_cast<int>(palabra.size()); ++i)
    {
        const int f = fila + (orientacion == 'V' ? i : 0);
        const int c = columna + (orientacion == 'H' ? i : 0);
        if (tabla[f][c]->isOcupada())
            return true;
        const int vecinos[][2] = {{f-1,c},{f+1,c},{f,c-1},{f,c+1}};
        for (const auto &v : vecinos)
            if (dentroDelTablero(v[0], v[1]) && tabla[v[0]][v[1]]->isOcupada())
                return true;
    }
    return false;
}

bool Tablero::jugadorTieneFichas(const Jugador *jugador, int fila, int columna,
                                 const std::string &palabra, char orientacion) const
{
    if (!jugador)
        return false;
    std::array<int, 26> requeridas{};
    for (int i = 0; i < static_cast<int>(palabra.size()); ++i)
    {
        int f = fila + (orientacion == 'V' ? i : 0);
        int c = columna + (orientacion == 'H' ? i : 0);
        if (!tabla[f][c]->isOcupada())
            ++requeridas[palabra[i] - 'A'];
    }
    ListaFicha *atril = const_cast<Jugador *>(jugador)->getFicha();
    for (int i = 0; i < 26; ++i)
        if (requeridas[i] > atril->contar(static_cast<char>('A' + i)))
            return false;
    return true;
}

std::string Tablero::construirCruce(int fila, int columna, char letra,
                                    char orientacion) const
{
    const bool cruceVertical = (orientacion == 'H');
    int df = cruceVertical ? 1 : 0;
    int dc = cruceVertical ? 0 : 1;
    int f = fila - df;
    int c = columna - dc;
    while (dentroDelTablero(f, c) && tabla[f][c]->isOcupada())
    {
        f -= df;
        c -= dc;
    }
    f += df;
    c += dc;

    std::string resultado;
    while (dentroDelTablero(f, c))
    {
        if (f == fila && c == columna)
            resultado.push_back(letra);
        else if (tabla[f][c]->isOcupada())
            resultado.push_back(tabla[f][c]->getLetra()->getLetra());
        else
            break;
        f += df;
        c += dc;
    }
    return resultado;
}

std::string Tablero::construirPalabraExtendida(int fila, int columna,
                                               const std::string &entrada,
                                               char orientacion) const
{
    const std::string palabra = normalizarPalabra(entrada);
    orientacion = mayuscula(orientacion);
    if (!coordenadasValidas(fila, columna, palabra.size(), orientacion))
        return {};
    const int df = orientacion == 'V' ? 1 : 0;
    const int dc = orientacion == 'H' ? 1 : 0;

    int inicioFila = fila;
    int inicioColumna = columna;
    while (dentroDelTablero(inicioFila - df, inicioColumna - dc) &&
           tabla[inicioFila - df][inicioColumna - dc]->isOcupada())
    {
        inicioFila -= df;
        inicioColumna -= dc;
    }

    std::string resultado;
    int f = inicioFila;
    int c = inicioColumna;
    while (dentroDelTablero(f, c))
    {
        int indice = orientacion == 'H' ? c - columna : f - fila;
        if (indice >= 0 && indice < static_cast<int>(palabra.size()))
            resultado.push_back(palabra[indice]);
        else if (tabla[f][c]->isOcupada())
            resultado.push_back(tabla[f][c]->getLetra()->getLetra());
        else
            break;
        f += df;
        c += dc;
    }
    return resultado;
}

bool Tablero::validarCruces(int fila, int columna, const std::string &entrada,
                            char orientacion) const
{
    const auto palabra = normalizarPalabra(entrada);
    orientacion = mayuscula(orientacion);
    if (!coordenadasValidas(fila, columna, palabra.size(), orientacion) ||
        !std::all_of(palabra.begin(), palabra.end(), [](char c) { return c >= 'A' && c <= 'Z'; }))
        return false;
    if (!diccionario)
        return true;
    for (int i = 0; i < static_cast<int>(palabra.size()); ++i)
    {
        int f = fila + (orientacion == 'V' ? i : 0);
        int c = columna + (orientacion == 'H' ? i : 0);
        if (tabla[f][c]->isOcupada())
            continue;
        std::string cruce = construirCruce(f, c, palabra[i], orientacion);
        if (cruce.size() > 1 && !diccionario->contiene(cruce))
            return false;
    }
    return true;
}

int Tablero::calcularPuntajePalabra(int fila, int columna,
                                    const std::string &palabra,
                                    char orientacion) const
{
    int subtotal = 0;
    int multiplicadorPalabra = 1;
    for (int i = 0; i < static_cast<int>(palabra.size()); ++i)
    {
        int f = fila + (orientacion == 'V' ? i : 0);
        int c = columna + (orientacion == 'H' ? i : 0);
        Cuadricula *casilla = tabla[f][c];
        if (casilla->isOcupada())
            subtotal += casilla->getLetra()->getPuntaje();
        else
        {
            Ficha temporal(palabra[i]);
            subtotal += temporal.getPuntaje() * casilla->getMultiplicadorLetra();
            multiplicadorPalabra *= casilla->getMultiplicadorPalabra();
        }
    }
    // Count adjacent existing letters without activating their bonuses again.
    const int df = orientacion == 'V' ? 1 : 0;
    const int dc = orientacion == 'H' ? 1 : 0;
    for (int f = fila - df, c = columna - dc;
         dentroDelTablero(f, c) && tabla[f][c]->isOcupada(); f -= df, c -= dc)
        subtotal += tabla[f][c]->getLetra()->getPuntaje();

    const int longitud = static_cast<int>(palabra.size());
    for (int f = fila + df * longitud, c = columna + dc * longitud;
         dentroDelTablero(f, c) && tabla[f][c]->isOcupada(); f += df, c += dc)
        subtotal += tabla[f][c]->getLetra()->getPuntaje();

    return subtotal * multiplicadorPalabra;
}

int Tablero::calcularPuntajeCruces(int fila, int columna,
                                   const std::string &palabra,
                                   char orientacion) const
{
    int total = 0;
    const int df = orientacion == 'H' ? 1 : 0;
    const int dc = orientacion == 'H' ? 0 : 1;
    for (int i = 0; i < static_cast<int>(palabra.size()); ++i)
    {
        int f = fila + (orientacion == 'V' ? i : 0);
        int c = columna + (orientacion == 'H' ? i : 0);
        Cuadricula *nueva = tabla[f][c];
        if (nueva->isOcupada())
            continue;
        std::string cruce = construirCruce(f, c, palabra[i], orientacion);
        if (cruce.size() <= 1)
            continue;

        int subtotal = 0;
        int rf = f;
        int rc = c;
        while (dentroDelTablero(rf - df, rc - dc) && tabla[rf - df][rc - dc]->isOcupada())
        {
            rf -= df;
            rc -= dc;
        }
        for (std::size_t j = 0; j < cruce.size(); ++j)
        {
            if (rf == f && rc == c)
            {
                Ficha temporal(palabra[i]);
                subtotal += temporal.getPuntaje() * nueva->getMultiplicadorLetra();
            }
            else
                subtotal += tabla[rf][rc]->getLetra()->getPuntaje();
            rf += df;
            rc += dc;
        }
        total += subtotal * nueva->getMultiplicadorPalabra();
    }
    return total;
}

bool Tablero::validarJugada(int fila, int columna, const std::string &palabra,
                            char orientacion, Jugador *jugador,
                            std::string &palabraCompleta, std::string *error) const
{
    auto fallar = [&](const std::string &mensaje) {
        if (error)
            *error = mensaje;
        return false;
    };

    if (!jugador)
        return fallar("El jugador no es válido.");
    if (palabra.size() < 2 || palabra.size() > MAX_LONGITUD_PALABRA)
        return fallar("La palabra debe tener entre 2 y 6 letras.");
    if (!std::all_of(palabra.begin(), palabra.end(), [](unsigned char c) { return c >= 'A' && c <= 'Z'; }))
        return fallar("La palabra solo puede contener letras de la A a la Z, sin tildes.");
    if (orientacion != 'H' && orientacion != 'V')
        return fallar("La orientación debe ser H o V.");

    if (!coordenadasValidas(fila, columna, palabra.size(), orientacion))
        return fallar("La palabra no cabe en el tablero.");

    int fichasNuevas = 0;
    for (int i = 0; i < static_cast<int>(palabra.size()); ++i)
    {
        int f = fila + (orientacion == 'V' ? i : 0);
        int c = columna + (orientacion == 'H' ? i : 0);
        Cuadricula *casilla = tabla[f][c];
        if (casilla->isOcupada())
        {
            if (!casilla->getLetra() || casilla->getLetra()->getLetra() != palabra[i])
                return fallar("La palabra entra en conflicto con una letra del tablero.");
        }
        else
            ++fichasNuevas;
    }
    if (fichasNuevas == 0)
        return fallar("Debe colocar al menos una ficha nueva.");
    if (!tieneConexion(fila, columna, palabra, orientacion))
        return fallar(tableroVacio()
                          ? "La primera palabra debe cruzar la casilla central (7,7)."
                          : "La palabra debe conectarse con alguna ficha existente.");
    if (!jugadorTieneFichas(jugador, fila, columna, palabra, orientacion))
        return fallar("El jugador no tiene todas las fichas necesarias.");

    palabraCompleta = construirPalabraExtendida(fila, columna, palabra, orientacion);
    if (diccionario && !diccionario->contiene(palabraCompleta))
        return fallar("La palabra '" + palabraCompleta + "' no está en el diccionario.");
    if (!validarCruces(fila, columna, palabra, orientacion))
        return fallar("Una de las palabras cruzadas no está en el diccionario.");

    if (error)
        error->clear();
    return true;
}

bool Tablero::jugarPalabra(int fila, int columna, const std::string &entrada,
                           char orientacion, Jugador *jugador,
                           std::string *error, int *puntajeObtenido)
{
    const std::string palabra = normalizarPalabra(entrada);
    orientacion = mayuscula(orientacion);
    if (puntajeObtenido)
        *puntajeObtenido = 0;
    std::string palabraCompleta;
    if (!validarJugada(fila, columna, palabra, orientacion, jugador, palabraCompleta, error))
        return false;

    int puntaje = calcularPuntajePalabra(fila, columna, palabra, orientacion) +
                  calcularPuntajeCruces(fila, columna, palabra, orientacion);

    for (int i = 0; i < static_cast<int>(palabra.size()); ++i)
    {
        int f = fila + (orientacion == 'V' ? i : 0);
        int c = columna + (orientacion == 'H' ? i : 0);
        if (tabla[f][c]->isOcupada())
            continue;
        Ficha *ficha = jugador->quitarFicha(palabra[i]);
        if (!ficha)
            throw std::logic_error("Error interno al retirar una ficha validada.");
        tabla[f][c]->setLetra(ficha);
    }

    jugador->incrementarPuntaje(puntaje);
    jugador->getPalabra()->ingresaPalabra(new Palabra(palabraCompleta, puntaje));
    registrarPalabraJugada(jugador->getNombre(), palabraCompleta, puntaje);
    reponerAtril(jugador);

    if (error)
        error->clear();
    if (puntajeObtenido)
        *puntajeObtenido = puntaje;
    return true;
}

void Tablero::palabraInicial(std::string palabra, char posicion, Jugador *jugador)
{
    palabra = normalizarPalabra(palabra);
    posicion = mayuscula(posicion);
    int fila = 7;
    int columna = 7;
    if (posicion == 'H')
        columna -= static_cast<int>(palabra.size()) / 2;
    else
        fila -= static_cast<int>(palabra.size()) / 2;
    std::string error;
    if (!jugarPalabra(fila, columna, palabra, posicion, jugador, &error))
        std::cout << "No se pudo colocar la palabra: " << error << '\n';
}

bool Tablero::palabraAprobada(int columna, int fila, Jugador *jugador,
                              std::string palabra, char orientacion)
{
    std::string palabraCompleta;
    return validarJugada(fila, columna, normalizarPalabra(palabra),
                         mayuscula(orientacion), jugador, palabraCompleta, nullptr);
}

bool Tablero::compararArchivo(std::string palabra) const
{
    return !diccionario || diccionario->contiene(normalizarPalabra(palabra));
}

bool Tablero::colocarPalabra(int x, int y, std::string palabra, char,
                             char posicion, Jugador *jugador)
{
    return acomodarPalabra(x, y, palabra, ' ', posicion, jugador);
}

void Tablero::acomodarPalabraInicial(std::string palabra, char posicion, Jugador *jugador)
{
    palabraInicial(std::move(palabra), posicion, jugador);
}

bool Tablero::acomodarPalabra(int fila, int columna, std::string palabra,
                              char, char posicion, Jugador *jugador)
{
    return jugarPalabra(fila, columna, palabra, posicion, jugador);
}

bool Tablero::intercambiarFichas(Jugador *jugador, const std::string &entrada,
                                 std::string *error)
{
    auto fallar = [&](const std::string &mensaje) {
        if (error)
            *error = mensaje;
        return false;
    };
    if (!jugador)
        return fallar("El jugador no es válido.");
    std::string letras = normalizarPalabra(entrada);
    if (letras.empty())
        return fallar("Debe indicar al menos una letra.");
    if (static_cast<int>(letras.size()) > jugador->getFicha()->getCantidad())
        return fallar("No puede cambiar más fichas de las que tiene.");
    if (bolsa->getCantidad() < static_cast<int>(letras.size()))
        return fallar("No hay suficientes fichas en la bolsa para realizar el cambio.");

    std::array<int, 26> solicitadas{};
    for (unsigned char c : letras)
    {
        if (c < 'A' || c > 'Z')
            return fallar("Solo se permiten letras de la A a la Z.");
        ++solicitadas[c - 'A'];
    }
    for (int i = 0; i < 26; ++i)
        if (solicitadas[i] > jugador->getFicha()->contar(static_cast<char>('A' + i)))
            return fallar("El atril no contiene todas las letras indicadas.");

    std::vector<Ficha *> retiradas;
    for (char letra : letras)
        retiradas.push_back(jugador->quitarFicha(letra));
    for (std::size_t i = 0; i < retiradas.size(); ++i)
        jugador->agregarFicha(bolsa->extraerFicha());
    for (Ficha *ficha : retiradas)
        bolsa->ingresaFicha(ficha);
    bolsa->barajar();

    if (error)
        error->clear();
    return true;
}

void Tablero::cambiarFichas(Jugador *jugador, const std::string &letras)
{
    std::string error;
    if (!intercambiarFichas(jugador, letras, &error))
        std::cout << "No se pudieron cambiar las fichas: " << error << '\n';
}

void Tablero::colocarPalabraEnTablero(int fila, int columna,
                                      const std::string &palabra,
                                      char orientacion, Jugador *jugador)
{
    std::string error;
    int puntos = 0;
    if (jugarPalabra(fila, columna, palabra, orientacion, jugador, &error, &puntos))
        std::cout << "Palabra colocada. Puntaje obtenido: " << puntos << ".\n";
    else
        std::cout << "No se pudo colocar la palabra: " << error << '\n';
}

void Tablero::registrarPalabraJugada(const std::string &jugador,
                                     const std::string &palabra, int puntaje)
{
    std::ostringstream registro;
    registro << jugador << ": " << palabra;
    if (puntaje > 0)
        registro << " (+" << puntaje << " puntos)";
    historial->agregarPalabra(registro.str());
}

std::string Tablero::obtenerHistorialPalabras() const { return historial->toString(); }

char Tablero::buscarLetra(int columna, int fila) const
{
    return extraerLetra(fila, columna);
}

char Tablero::posicion(int columna, int fila) const
{
    if (!dentroDelTablero(fila, columna))
        return 'N';
    bool horizontal = (dentroDelTablero(fila, columna - 1) && tabla[fila][columna - 1]->isOcupada()) ||
                      (dentroDelTablero(fila, columna + 1) && tabla[fila][columna + 1]->isOcupada());
    bool vertical = (dentroDelTablero(fila - 1, columna) && tabla[fila - 1][columna]->isOcupada()) ||
                    (dentroDelTablero(fila + 1, columna) && tabla[fila + 1][columna]->isOcupada());
    if (horizontal && !vertical)
        return 'V';
    if (vertical && !horizontal)
        return 'H';
    return horizontal ? 'H' : (vertical ? 'V' : 'N');
}

char Tablero::extraerLetra(int fila, int columna) const
{
    if (!dentroDelTablero(fila, columna) || !tabla[fila][columna]->isOcupada() ||
        !tabla[fila][columna]->getLetra())
        return ' ';
    return tabla[fila][columna]->getLetra()->getLetra();
}

int Tablero::determinarPuntaje(Jugador *, Ficha *ficha, char letra,
                               bool especial, std::string bono) const
{
    Ficha temporal(letra);
    int puntos = ficha ? ficha->getPuntaje() : temporal.getPuntaje();
    if (!especial)
        return puntos;
    if (bono == "Doble letra")
        return puntos * 2;
    if (bono == "Triple letra")
        return puntos * 3;
    return puntos;
}

bool Tablero::ganador(int pasar, bool sinLetras)
{
    return (numJugadores > 0 && pasar >= numJugadores * 2) || sinLetras;
}

bool Tablero::hayGanador(int pasar, bool sinLetras)
{
    bool atrilVacio = false;
    for (int i = 0; i < jugadores->getCantidad(); ++i)
        atrilVacio = atrilVacio || jugadores->getJugador(i)->getFicha()->getCantidad() == 0;
    return ganador(pasar, sinLetras || (bolsa->getCantidad() == 0 && atrilVacio));
}

void Tablero::aplicarPuntajeFinal()
{
    int puntosRestantes = 0;
    Jugador *jugadorSinFichas = nullptr;
    for (int i = 0; i < jugadores->getCantidad(); ++i)
    {
        Jugador *jugador = jugadores->getJugador(i);
        int resta = 0;
        for (int j = 0; j < jugador->getFicha()->getCantidad(); ++j)
            resta += jugador->getFicha()->getLetra(j)->getPuntaje();
        jugador->incrementarPuntaje(-resta);
        puntosRestantes += resta;
        if (jugador->getFicha()->getCantidad() == 0)
            jugadorSinFichas = jugador;
    }
    if (jugadorSinFichas)
        jugadorSinFichas->incrementarPuntaje(puntosRestantes);
}

void Tablero::juegoCompleto()
{
    mostrar();
    std::cout << "Use GestorJuego para ejecutar el flujo interactivo completo.\n";
}

std::string Tablero::toString() const
{
    return toString(false);
}

std::string Tablero::toString(bool colores) const
{
    constexpr int ANCHO_CELDA = 5;
    const std::string vacia(ANCHO_CELDA, ' ');
    const std::string borde(ANCHO_CELDA, '-');
    const auto centrar = [&](const std::string &texto) {
        const int espacios = ANCHO_CELDA - static_cast<int>(texto.size());
        return std::string(espacios / 2, ' ') + texto +
               std::string(espacios - espacios / 2, ' ');
    };
    std::ostringstream salida;
    const auto verde = colores ? VERDE : "";
    const auto reset = colores ? RESET : "";
    salida << "--- TABLERO ACTUAL ---\n";
    salida << verde << "   ";
    for (int columna = 0; columna < TAMANIO; ++columna)
        salida << centrar(std::to_string(columna)) << ' ';
    salida << reset << '\n';

    const auto separador = [&]() {
        salida << verde << "  +";
        for (int columna = 0; columna < TAMANIO; ++columna)
            salida << borde << '+';
        salida << reset << '\n';
    };

    for (int fila = 0; fila < TAMANIO; ++fila)
    {
        separador();
        salida << verde << "  |" << reset;
        for (int columna = 0; columna < TAMANIO; ++columna)
        {
            const Cuadricula *casilla = tabla[fila][columna];
            if (casilla->isOcupada() && casilla->getLetra())
            {
                if (colores) salida << FICHA;
                salida << centrar(std::string(1, casilla->getLetra()->getLetra()));
                salida << reset;
            }
            else
            {
                const std::string bono = codigoBono(casilla);
                const char *color = colorBono(bono);
                if (colores && color)
                    salida << color << vacia << reset;
                else
                    salida << centrar(bono);
            }
            salida << verde << '|' << reset;
        }
        salida << verde << ' ' << std::setw(2) << fila << reset << '\n';
    }
    separador();
    return salida.str();
}

void Tablero::mostrar() const
{
    mostrar(Consola::soportaColor());
}

void Tablero::mostrar(bool colores) const
{
    std::cout << toString(colores);
    if (colores)
    {
        std::cout << ROJO << "   " << RESET << " 3P   "
                  << MAGENTA << "   " << RESET << " 2P   "
                  << AZUL << "   " << RESET << " 3L   "
                  << CELESTE << "   " << RESET << " 2L   "
                  << AMARILLO << "   " << RESET << " Centro\n";
    }
    else
        std::cout << "Leyenda: ** centro, 2P/3P palabra, 2L/3L letra.\n";
}

std::string Tablero::toStringJugador() const { return jugadores->toStringJugador(); }
