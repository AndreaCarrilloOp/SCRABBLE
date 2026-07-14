#include "Tablero.h"
#include <cassert>
#include <iostream>
#include <string>
#include <unordered_set>

class DiccionarioPrueba final : public IDiccionarioPalabras
{
private:
    std::unordered_set<std::string> palabras;

public:
    DiccionarioPrueba(std::initializer_list<std::string> valores) : palabras(valores) {}
    bool contiene(const std::string &palabra) const override
    {
        return palabras.find(palabra) != palabras.end();
    }
};

void agregarFichas(Jugador &jugador, const std::string &letras)
{
    for (char letra : letras)
        jugador.agregarFicha(new Ficha(letra));
}

int main()
{
    DiccionarioPrueba diccionario({"CASA", "SOL", "OSO", "ALA"});

    {
        Tablero tablero(&diccionario);
        Jugador jugador("Andrea");
        agregarFichas(jugador, "CASA");

        std::string error;
        int puntos = 0;
        assert(tablero.jugarPalabra(7, 5, "casa", 'h', &jugador, &error, &puntos));
        assert(error.empty());
        assert(puntos == 12); // 6 puntos x doble palabra del centro.
        assert(jugador.getPuntaje() == 12);
        assert(tablero.extraerLetra(7, 5) == 'C');
        assert(tablero.extraerLetra(7, 8) == 'A');
        assert(jugador.getFicha()->getCantidad() == 0);

        agregarFichas(jugador, "OL");
        assert(tablero.jugarPalabra(7, 7, "SOL", 'V', &jugador, &error, &puntos));
        assert(puntos == 3);
        assert(tablero.extraerLetra(8, 7) == 'O');
        assert(tablero.extraerLetra(9, 7) == 'L');
        assert(jugador.getPuntaje() == 15);
    }

    {
        Tablero tablero(&diccionario);
        Jugador jugador("Prueba");
        agregarFichas(jugador, "CASAOSO");
        std::string error;
        assert(!tablero.jugarPalabra(0, 0, "CASA", 'H', &jugador, &error));
        assert(error.find("central") != std::string::npos);
        assert(!tablero.jugarPalabra(7, 6, "MAL", 'H', &jugador, &error));
        assert(error.find("fichas") != std::string::npos ||
               error.find("diccionario") != std::string::npos);
    }

    {
        Tablero tablero(&diccionario);
        Jugador jugador("Cambio");
        agregarFichas(jugador, "AB");
        tablero.getLista()->ingresaFicha(new Ficha('X'));
        tablero.getLista()->ingresaFicha(new Ficha('Y'));
        std::string error;
        assert(tablero.intercambiarFichas(&jugador, "A", &error));
        assert(jugador.getFicha()->getCantidad() == 2);
        assert(tablero.getLista()->getCantidad() == 2);
        assert(!tablero.intercambiarFichas(&jugador, "ZZ", &error));
    }

    std::cout << "Todas las pruebas pasaron correctamente.\n";
    return 0;
}
