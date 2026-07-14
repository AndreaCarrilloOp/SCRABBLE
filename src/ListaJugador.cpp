#include "ListaJugador.h"
#include <fstream>
#include <sstream>

ListaJugador::ListaJugador() : cabeza(nullptr), cantidad(0) {}
ListaJugador::~ListaJugador()
{
    while (cabeza)
    {
        Nodo *actual = cabeza;
        cabeza = cabeza->siguiente;
        delete actual->jugador;
        delete actual;
    }
}
int ListaJugador::getCantidad() const { return cantidad; }
Jugador *ListaJugador::getJugador(int indice) const
{
    if (indice < 0 || indice >= cantidad)
        return nullptr;
    Nodo *actual = cabeza;
    for (int i = 0; i < indice; ++i)
        actual = actual->siguiente;
    return actual->jugador;
}
void ListaJugador::ingresaJugadorInicio(Jugador *jugador)
{
    if (!jugador)
        return;
    Nodo *nuevo = new Nodo(jugador);
    nuevo->siguiente = cabeza;
    cabeza = nuevo;
    ++cantidad;
}
void ListaJugador::ingresaJugadorFinal(Jugador *jugador)
{
    if (!jugador)
        return;
    Nodo *nuevo = new Nodo(jugador);
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
bool ListaJugador::eliminarJugador(const std::string &nombre)
{
    Nodo *anterior = nullptr;
    Nodo *actual = cabeza;
    while (actual)
    {
        if (actual->jugador->getNombre() == nombre)
        {
            if (anterior)
                anterior->siguiente = actual->siguiente;
            else
                cabeza = actual->siguiente;
            delete actual->jugador;
            delete actual;
            --cantidad;
            return true;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    return false;
}
Jugador *ListaJugador::buscarJugador(const std::string &nombre) const
{
    for (Nodo *actual = cabeza; actual; actual = actual->siguiente)
        if (actual->jugador->getNombre() == nombre)
            return actual->jugador;
    return nullptr;
}
int ListaJugador::obtenerIndiceJugador(const std::string &nombre) const
{
    int indice = 0;
    for (Nodo *actual = cabeza; actual; actual = actual->siguiente, ++indice)
        if (actual->jugador->getNombre() == nombre)
            return indice;
    return -1;
}
void ListaJugador::ordenarPorPuntaje() { cabeza = mergeSort(cabeza); }
ListaJugador::Nodo *ListaJugador::mergeSort(Nodo *head)
{
    if (!head || !head->siguiente)
        return head;
    Nodo *medio = getMiddle(head);
    Nodo *derecha = medio->siguiente;
    medio->siguiente = nullptr;
    return merge(mergeSort(head), mergeSort(derecha));
}
ListaJugador::Nodo *ListaJugador::merge(Nodo *izquierda, Nodo *derecha)
{
    if (!izquierda)
        return derecha;
    if (!derecha)
        return izquierda;
    if (izquierda->jugador->getPuntaje() >= derecha->jugador->getPuntaje())
    {
        izquierda->siguiente = merge(izquierda->siguiente, derecha);
        return izquierda;
    }
    derecha->siguiente = merge(izquierda, derecha->siguiente);
    return derecha;
}
ListaJugador::Nodo *ListaJugador::getMiddle(Nodo *head)
{
    if (!head)
        return nullptr;
    Nodo *lento = head;
    Nodo *rapido = head->siguiente;
    while (rapido && rapido->siguiente)
    {
        lento = lento->siguiente;
        rapido = rapido->siguiente->siguiente;
    }
    return lento;
}
void ListaJugador::guardarJugadores(const std::string &ruta) const
{
    std::ofstream archivo(ruta);
    for (Nodo *actual = cabeza; actual; actual = actual->siguiente)
        actual->jugador->save(archivo);
}
void ListaJugador::cargarJugadores(const std::string &ruta)
{
    std::ifstream archivo(ruta);
    while (archivo)
    {
        std::string nombre;
        int puntaje;
        if (!std::getline(archivo >> std::ws, nombre))
            break;
        if (!(archivo >> puntaje))
            break;
        ingresaJugadorFinal(new Jugador(nombre, puntaje));
    }
}
std::string ListaJugador::toString() const
{
    std::ostringstream salida;
    for (Nodo *actual = cabeza; actual; actual = actual->siguiente)
        salida << actual->jugador->toString() << '\n';
    return salida.str();
}
std::string ListaJugador::toStringJugador() const
{
    std::ostringstream salida;
    int posicion = 1;
    for (Nodo *actual = cabeza; actual; actual = actual->siguiente)
        salida << posicion++ << ". " << actual->jugador->toString() << '\n';
    return salida.str();
}
