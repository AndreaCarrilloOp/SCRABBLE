#include "ListaPalabra.h"
#include <fstream>
#include <sstream>

ListaPalabra::ListaPalabra() : cabeza(nullptr), cantidad(0) {}
ListaPalabra::~ListaPalabra()
{
    while (cabeza)
    {
        Nodo *actual = cabeza;
        cabeza = cabeza->siguiente;
        delete actual->palabra;
        delete actual;
    }
}
int ListaPalabra::getCantidad() const { return cantidad; }
Palabra *ListaPalabra::getPalabra(int indice) const
{
    if (indice < 0 || indice >= cantidad)
        return nullptr;
    Nodo *actual = cabeza;
    for (int i = 0; i < indice; ++i)
        actual = actual->siguiente;
    return actual->palabra;
}
void ListaPalabra::ingresaPalabra(Palabra *palabra)
{
    if (!palabra)
        return;
    Nodo *nuevo = new Nodo(palabra);
    if (!cabeza)
        cabeza = nuevo;
    else
    {
        Nodo *actual = cabeza;
        while (actual->siguiente)
            actual = actual->siguiente;
        actual->siguiente = nuevo;
    }
    ++cantidad;
}
bool ListaPalabra::eliminarPalabra(const std::string &texto)
{
    Nodo *anterior = nullptr;
    Nodo *actual = cabeza;
    while (actual)
    {
        if (actual->palabra->getPalabra() == texto)
        {
            if (anterior)
                anterior->siguiente = actual->siguiente;
            else
                cabeza = actual->siguiente;
            delete actual->palabra;
            delete actual;
            --cantidad;
            return true;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    return false;
}
bool ListaPalabra::buscarPalabra(const std::string &texto) const
{
    for (Nodo *actual = cabeza; actual; actual = actual->siguiente)
        if (actual->palabra->getPalabra() == texto)
            return true;
    return false;
}
void ListaPalabra::guardarPalabras(int jugadorId) const
{
    std::ofstream archivo("palabras_jugador_" + std::to_string(jugadorId) + ".txt");
    for (Nodo *actual = cabeza; actual; actual = actual->siguiente)
        actual->palabra->guardar(archivo);
}
void ListaPalabra::cargarPalabras(int jugadorId)
{
    std::ifstream archivo("palabras_jugador_" + std::to_string(jugadorId) + ".txt");
    std::string texto;
    int puntaje;
    while (archivo >> texto >> puntaje)
        ingresaPalabra(new Palabra(texto, puntaje));
}
std::string ListaPalabra::toString() const
{
    std::ostringstream salida;
    if (!cabeza)
        return "No hay palabras registradas.";
    int numero = 1;
    for (Nodo *actual = cabeza; actual; actual = actual->siguiente)
        salida << numero++ << ". " << actual->palabra->toString() << '\n';
    return salida.str();
}
std::string ListaPalabra::toStringJugador() const { return toString(); }
void ListaPalabra::manejarLetrasCompartidas(Palabra *) {}
bool ListaPalabra::verificarPalabraEnBaseDeDatos(const std::string &texto) const
{
    std::ifstream archivo("Datos.txt");
    std::string palabra;
    while (archivo >> palabra)
        if (palabra == texto)
            return true;
    return false;
}
