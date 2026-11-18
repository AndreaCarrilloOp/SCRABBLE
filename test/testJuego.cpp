#include "Tablero.h"
#include <stdexcept>
#include <limits>
#include <iostream>
#include <string>
#include <unordered_set>

// These checks also run in Release builds.
#define CHECK(expr) do { if (!(expr)) throw std::runtime_error( \
    std::string("Check failed at line ") + std::to_string(__LINE__) + ": " #expr); } while (false)

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
    DiccionarioPrueba diccionario({"CASA", "CASAS", "SOL", "OSO", "ALA", "MALA"});

    {
        Tablero tablero(&diccionario);
        Jugador jugador("Andrea");
        agregarFichas(jugador, "CASA");

        std::string error;
        int puntos = 0;
        CHECK(tablero.jugarPalabra(7, 5, "casa", 'h', &jugador, &error, &puntos));
        CHECK(error.empty());
        CHECK(puntos == 12); // 6 puntos x doble palabra del centro.
        CHECK(jugador.getPuntaje() == 12);
        CHECK(tablero.extraerLetra(7, 5) == 'C');
        CHECK(tablero.extraerLetra(7, 8) == 'A');
        CHECK(jugador.getFicha()->getCantidad() == 0);

        agregarFichas(jugador, "OL");
        CHECK(tablero.jugarPalabra(7, 7, "SOL", 'V', &jugador, &error, &puntos));
        CHECK(puntos == 3);
        CHECK(tablero.extraerLetra(8, 7) == 'O');
        CHECK(tablero.extraerLetra(9, 7) == 'L');
        CHECK(jugador.getPuntaje() == 15);
    }

    {
        Tablero tablero(&diccionario);
        Jugador jugador("Prueba");
        agregarFichas(jugador, "CASAOSO");
        std::string error;
        CHECK(!tablero.jugarPalabra(0, 0, "CASA", 'H', &jugador, &error));
        CHECK(error.find("central") != std::string::npos);
        CHECK(!tablero.jugarPalabra(7, 6, "MAL", 'H', &jugador, &error));
        CHECK(error.find("fichas") != std::string::npos ||
               error.find("diccionario") != std::string::npos);
    }

    {
        Tablero tablero(&diccionario);
        Jugador jugador("Cambio");
        agregarFichas(jugador, "AB");
        tablero.getLista()->ingresaFicha(new Ficha('X'));
        tablero.getLista()->ingresaFicha(new Ficha('Y'));
        std::string error;
        CHECK(tablero.intercambiarFichas(&jugador, "A", &error));
        CHECK(jugador.getFicha()->getCantidad() == 2);
        CHECK(tablero.getLista()->getCantidad() == 2);
        CHECK(!tablero.intercambiarFichas(&jugador, "ZZ", &error));
    }

    // Existing prefixes and suffixes count toward the whole word.
    {
        Tablero tablero(&diccionario);
        Jugador jugador("Extension");
        agregarFichas(jugador, "CASAS");
        int puntos = 0;
        CHECK(tablero.jugarPalabra(7, 5, "CASA", 'H', &jugador));
        CHECK(tablero.jugarPalabra(7, 8, "AS", 'H', &jugador, nullptr, &puntos));
        CHECK(puntos == 7);
        CHECK(jugador.getPuntaje() == 19);
        CHECK(tablero.extraerLetra(7, 9) == 'S');
        CHECK(!tablero.palabraAprobada(5, 7, &jugador, "CASA"));
    }
    {
        Tablero tablero(&diccionario);
        Jugador jugador("Prefijo");
        agregarFichas(jugador, "ALAM");
        CHECK(tablero.jugarPalabra(7, 7, "ALA", 'H', &jugador));
        int puntos = 0;
        CHECK(tablero.palabraAprobada(6, 7, &jugador, "MA"));
        CHECK(tablero.jugarPalabra(7, 6, "MA", 'H', &jugador, nullptr, &puntos));
        CHECK(puntos == 6);
    }

    // Invalid moves leave board, score, rack and history unchanged.
    {
        Tablero tablero(&diccionario);
        Jugador jugador("Validacion");
        agregarFichas(jugador, "CASA");
        const auto antes = tablero.toString();
        const auto historial = tablero.obtenerHistorialPalabras();
        int puntos = 999;
        CHECK(!tablero.jugarPalabra(std::numeric_limits<int>::max(), 7, "CASA", 'V',
                                 &jugador, nullptr, &puntos));
        CHECK(puntos == 0);
        CHECK(!tablero.palabraAprobada(5, 7, &jugador, "C@SA"));
        CHECK(!tablero.palabraAprobada(5, 7, &jugador, "C"));
        CHECK(!tablero.palabraAprobada(5, 7, &jugador, "CASA", 'X'));
        CHECK(tablero.construirPalabraExtendida(7, 5, "CASA", 'X').empty());
        CHECK(!tablero.validarCruces(-1, 0, "CASA", 'H'));
        CHECK(tablero.toString() == antes);
        CHECK(tablero.obtenerHistorialPalabras() == historial);
        CHECK(jugador.getPuntaje() == 0 && jugador.getFicha()->getCantidad() == 4);
    }

    // The reference board has 225 cells and five premium colors.
    {
        Tablero tablero;
        int triplePalabra = 0, doblePalabra = 0, tripleLetra = 0, dobleLetra = 0, centro = 0;
        for (int f = 0; f < Tablero::TAMANIO; ++f)
            for (int c = 0; c < Tablero::TAMANIO; ++c)
            {
                const auto *celda = tablero.getCuadricula(f, c);
                if (celda->getBono() == "Centro") ++centro;
                else if (celda->getMultiplicadorPalabra() == 3) ++triplePalabra;
                else if (celda->getMultiplicadorPalabra() == 2) ++doblePalabra;
                else if (celda->getMultiplicadorLetra() == 3) ++tripleLetra;
                else if (celda->getMultiplicadorLetra() == 2) ++dobleLetra;
                const auto *opuesta = tablero.getCuadricula(14 - f, 14 - c);
                CHECK(celda->getBono() == opuesta->getBono());
            }
        CHECK(triplePalabra == 8 && doblePalabra == 16 && tripleLetra == 12 &&
              dobleLetra == 24 && centro == 1);
        CHECK(tablero.getCuadricula(7, 7)->getBono() == "Centro");
        const auto plano = tablero.toString(false);
        const auto color = tablero.toString(true);
        CHECK(plano.find('\033') == std::string::npos);
        CHECK(plano.find("3P") != std::string::npos);
        CHECK(plano.find("TABLERO ACTUAL") != std::string::npos);
        const auto contar = [&](const std::string &fragmento) {
            int n = 0;
            for (std::size_t pos = 0; (pos = color.find(fragmento, pos)) != std::string::npos;
                 pos += fragmento.size()) ++n;
            return n;
        };
        CHECK(contar("\033[48;2;195;28;40m") == 8);
        CHECK(contar("\033[48;2;151;20;144m") == 16);
        CHECK(contar("\033[48;2;43;107;220m") == 12);
        CHECK(contar("\033[48;2;105;207;210m") == 24);
        CHECK(contar("\033[48;2;245;221;112m") == 1);
    }

    // Linked-list tail stays valid after removal and shuffling.
    {
        ListaFicha lista;
        lista.ingresaFicha(new Ficha('A'));
        lista.ingresaFicha(new Ficha('B'));
        lista.ingresaFicha(new Ficha('C'));
        CHECK(lista.eliminarFicha('C'));
        lista.ingresaFicha(new Ficha('D'));
        CHECK(lista.getLetra(2)->getLetra() == 'D');
        delete lista.eliminarRetornar(2);
        lista.ingresaFicha(new Ficha('E'));
        lista.barajar();
        CHECK(lista.getCantidad() == 3);
        CHECK(lista.contar('A') == 1 && lista.contar('B') == 1 && lista.contar('E') == 1);
        while (lista.getCantidad()) delete lista.extraerFicha();
        lista.ingresaFicha(new Ficha('Z'));
        CHECK(lista.getLetra(0)->getLetra() == 'Z');
    }

    std::cout << "Todas las pruebas pasaron correctamente.\n";
    return 0;
}
