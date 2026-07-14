#include "GestorJuego.h"
#include <exception>
#include <iostream>

int main()
{
    try
    {
        GestorJuego gestor;
        gestor.inicializarJuego();
        gestor.ejecutarJuego();
        return 0;
    }
    catch (const std::exception &error)
    {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}
