#include "ListaFicha.h"
#include <algorithm>
#include <fstream>
#include <random>
#include <sstream>
#include <vector>

ListaFicha::ListaFicha() : cabeza(nullptr), cantidad(0) {}

ListaFicha::~ListaFicha()
{
    while (cabeza)
    {
        Nodo *actual = cabeza;
        cabeza = cabeza->siguiente;
        delete actual->ficha;
        delete actual;
    }
}

int ListaFicha::getCantidad() const { return cantidad; }

Ficha *ListaFicha::getLetra(int indice) const
{
    if (indice < 0 || indice >= cantidad)
        return nullptr;
    Nodo *actual = cabeza;
    for (int i = 0; i < indice; ++i)
        actual = actual->siguiente;
    return actual->ficha;
}

void ListaFicha::ingresaFicha(Ficha *ficha)
{
    if (!ficha)
        return;
    Nodo *nuevo = new Nodo(ficha);
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

bool ListaFicha::eliminarFicha(char letra)
{
    Ficha *ficha = eliminarFichaEspecifica(letra);
    if (!ficha)
        return false;
    delete ficha;
    return true;
}

Ficha *ListaFicha::eliminarFichaEspecifica(char letra)
{
    letra = static_cast<char>(std::toupper(static_cast<unsigned char>(letra)));
    Nodo *anterior = nullptr;
    Nodo *actual = cabeza;
    while (actual)
    {
        if (actual->ficha->getLetra() == letra)
        {
            if (anterior)
                anterior->siguiente = actual->siguiente;
            else
                cabeza = actual->siguiente;
            Ficha *resultado = actual->ficha;
            delete actual;
            --cantidad;
            return resultado;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    return nullptr;
}

Ficha *ListaFicha::eliminarRetornar(int indice)
{
    if (indice < 0 || indice >= cantidad)
        return nullptr;
    Nodo *anterior = nullptr;
    Nodo *actual = cabeza;
    for (int i = 0; i < indice; ++i)
    {
        anterior = actual;
        actual = actual->siguiente;
    }
    if (anterior)
        anterior->siguiente = actual->siguiente;
    else
        cabeza = actual->siguiente;
    Ficha *resultado = actual->ficha;
    delete actual;
    --cantidad;
    return resultado;
}

Ficha *ListaFicha::extraerFicha()
{
    if (cantidad == 0)
        return nullptr;
    static std::mt19937 generador(std::random_device{}());
    std::uniform_int_distribution<int> distribucion(0, cantidad - 1);
    return eliminarRetornar(distribucion(generador));
}

void ListaFicha::barajar()
{
    if (cantidad < 2)
        return;
    std::vector<Ficha *> fichas;
    while (cantidad > 0)
        fichas.push_back(eliminarRetornar(0));
    static std::mt19937 generador(std::random_device{}());
    std::shuffle(fichas.begin(), fichas.end(), generador);
    for (Ficha *ficha : fichas)
        ingresaFicha(ficha);
}

bool ListaFicha::encuentrada(char letra) const { return contar(letra) > 0; }
bool ListaFicha::seEncuentra(char letra) const { return encuentrada(letra); }

int ListaFicha::contar(char letra) const
{
    letra = static_cast<char>(std::toupper(static_cast<unsigned char>(letra)));
    int total = 0;
    for (Nodo *actual = cabeza; actual; actual = actual->siguiente)
        if (actual->ficha->getLetra() == letra)
            ++total;
    return total;
}

void ListaFicha::guardarLetras(const std::string &ruta) const
{
    std::ofstream archivo(ruta);
    for (Nodo *actual = cabeza; actual; actual = actual->siguiente)
        actual->ficha->guardar(archivo);
}

void ListaFicha::cargarLetras(const std::string &ruta)
{
    std::ifstream archivo(ruta);
    char letra;
    int cantidadLeida;
    while (archivo >> letra >> cantidadLeida)
        ingresaFicha(new Ficha(letra, cantidadLeida));
}

std::string ListaFicha::toString() const
{
    if (!cabeza)
        return "(sin fichas)";
    std::ostringstream salida;
    bool primera = true;
    for (Nodo *actual = cabeza; actual; actual = actual->siguiente)
    {
        if (!primera)
            salida << ' ';
        salida << actual->ficha->getLetra() << '(' << actual->ficha->getPuntaje() << ')';
        primera = false;
    }
    return salida.str();
}

std::string ListaFicha::toStringJugador() const
{
    std::ostringstream salida;
    salida << "Atril (" << cantidad << '/' << 6 << "): ";
    if (!cabeza)
        salida << "(vacío)";
    else
    {
        for (Nodo *actual = cabeza; actual; actual = actual->siguiente)
            salida << '[' << actual->ficha->getLetra() << ':'
                   << actual->ficha->getPuntaje() << "] ";
    }
    return salida.str();
}
