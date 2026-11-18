#include "GestorJuego.h"
#include "Consola.h"
#include <string>
#include <exception>
#include <iostream>

int main(int argc, char *argv[])
{
    try
    {
        if (argc == 2)
        {
            const std::string opcion = argv[1];
            if (opcion == "--preview" || opcion == "--preview-color" || opcion == "--preview-plain")
            {
                Consola::configurarConsola();
                Tablero tablero;
                tablero.mostrar(opcion == "--preview-color" ||
                                (opcion == "--preview" && Consola::soportaColor()));
                return 0;
            }
        }
        if (argc != 1)
        {
            std::cerr << "Uso: SCRABBLE [--preview|--preview-color|--preview-plain]\n";
            return 1;
        }
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
