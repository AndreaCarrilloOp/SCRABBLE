#include "Ficha.h"
#include <cctype>

Ficha::Ficha() : letra(' '), cantidad(0) {}

Ficha::Ficha(char letra, int cantidad)
    : letra(static_cast<char>(std::toupper(static_cast<unsigned char>(letra)))),
      cantidad(cantidad)
{
}

void Ficha::setLetra(char nuevaLetra)
{
    letra = static_cast<char>(std::toupper(static_cast<unsigned char>(nuevaLetra)));
}

char Ficha::getLetra() const { return letra; }
void Ficha::setCantidad(int nuevaCantidad) { cantidad = nuevaCantidad; }
int Ficha::getCantidad() const { return cantidad; }

int Ficha::getPuntaje() const
{
    switch (letra)
    {
    case 'A': case 'E': case 'I': case 'L': case 'N':
    case 'O': case 'R': case 'S': case 'T': case 'U':
        return 1;
    case 'D': case 'G':
        return 2;
    case 'B': case 'C': case 'M': case 'P':
        return 3;
    case 'F': case 'H': case 'V': case 'Y':
        return 4;
    case 'Q':
        return 5;
    case 'J': case 'X':
        return 8;
    case 'Z':
        return 10;
    default:
        return 0;
    }
}

void Ficha::guardar(std::ofstream &archivo) const
{
    archivo << letra << ' ' << cantidad << '\n';
}

void Ficha::leer(std::ifstream &archivo)
{
    archivo >> letra >> cantidad;
    setLetra(letra);
}

std::string Ficha::imprimir() const
{
    std::ostringstream salida;
    salida << letra << " (" << getPuntaje() << " pts)";
    return salida.str();
}

bool Ficha::esLetraCompartida(char otraLetra) const
{
    return letra == static_cast<char>(std::toupper(static_cast<unsigned char>(otraLetra)));
}
