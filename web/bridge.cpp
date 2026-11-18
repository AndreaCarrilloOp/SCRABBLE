#include "Tablero.h"
#include "FabricaDiccionarioPalabras.h"
#include <algorithm>
#include <array>
#include <exception>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

// The browser adapter owns a single local multiplayer session.
// All placement, dictionary, premium and scoring rules stay in Tablero.
namespace {
std::unique_ptr<IDiccionarioPalabras> dictionary;
std::unique_ptr<Tablero> game;
int active = 0;
int passes = 0;
bool finished = false;
std::string response;

std::string quote(const std::string &s)
{
    std::string result = "\"";
    const char *hex = "0123456789abcdef";
    for (unsigned char c : s) {
        if (c == '"' || c == '\\') {
            result += '\\';
            result += static_cast<char>(c);
        } else if (c < 32) {
            result += "\\u00";
            result += hex[c >> 4];
            result += hex[c & 15];
        } else {
            result += static_cast<char>(c);
        }
    }
    return result + '"';
}

std::string premium(const Cuadricula &cell)
{
    if (cell.getBono() == "Centro") return "**";
    if (cell.getMultiplicadorPalabra() == 3) return "3P";
    if (cell.getMultiplicadorPalabra() == 2) return "2P";
    if (cell.getMultiplicadorLetra() == 3) return "3L";
    if (cell.getMultiplicadorLetra() == 2) return "2L";
    return "";
}

std::string snapshot()
{
    std::ostringstream out;
    out << "{\"started\":" << (game ? "true" : "false");
    out << ",\"finished\":" << (finished ? "true" : "false");
    out << ",\"active\":" << active << ",\"passes\":" << passes;
    Tablero empty;
    const Tablero &board = game ? *game : empty;
    out << ",\"board\":[";
    for (int row = 0; row < Tablero::TAMANIO; ++row) {
        for (int col = 0; col < Tablero::TAMANIO; ++col) {
            if (row || col) out << ',';
            const auto *cell = board.getCuadricula(row, col);
            const auto *tile = cell->getLetra();
            out << "{\"bonus\":" << quote(premium(*cell))
                << ",\"letter\":" << quote(tile ? std::string(1, tile->getLetra()) : "")
                << ",\"points\":" << (tile ? tile->getPuntaje() : 0) << '}';
        }
    }
    out << "],\"players\":[";
    if (game) {
        for (int i = 0; i < game->getJugadores()->getCantidad(); ++i) {
            if (i) out << ',';
            const auto *p = game->getJugadores()->getJugador(i);
            out << "{\"name\":" << quote(p->getNombre())
                << ",\"score\":" << p->getPuntaje() << '}';
        }
    }
    out << "],\"rack\":[";
    if (game && !finished) {
        const auto *rack = game->getJugadores()->getJugador(active)->getFicha();
        for (int i = 0; i < rack->getCantidad(); ++i) {
            if (i) out << ',';
            const auto *tile = rack->getLetra(i);
            out << "{\"letter\":" << quote(std::string(1, tile->getLetra()))
                << ",\"points\":" << tile->getPuntaje() << '}';
        }
    }
    out << "],\"bag\":" << (game ? game->getLista()->getCantidad() : 0)
        << ",\"history\":" << quote(game ? game->obtenerHistorialPalabras() : "")
        << '}';
    return out.str();
}

const char *result(bool ok, const std::string &message, bool advanced = false)
{
    response = std::string("{\"ok\":") + (ok ? "true" : "false") +
        ",\"advanced\":" + (advanced ? "true" : "false") +
        ",\"message\":" + quote(message) + ",\"state\":" + snapshot() + '}';
    return response.c_str();
}

template <typename F> const char *guard(F action)
{
    try { return action(); }
    catch (const std::exception &e) { return result(false, e.what()); }
    catch (...) { return result(false, "Error inesperado del motor."); }
}

bool running() { return game && !finished; }
Jugador *player() { return game->getJugadores()->getJugador(active); }

void advance()
{
    if (game->hayGanador(passes, false)) {
        game->aplicarPuntajeFinal();
        finished = true;
    } else {
        active = (active + 1) % game->getJugadores()->getCantidad();
    }
}
} // namespace

extern "C" {
const char *scrabble_state()
{
    return guard([] { return result(true, ""); });
}

const char *scrabble_start(int count, const char *n1, const char *n2,
                           const char *n3, const char *n4)
{
    return guard([&] {
        if (count < 2 || count > 4) return result(false, "Seleccione de 2 a 4 jugadores.");
        const std::array<const char *, 4> names{n1, n2, n3, n4};
        std::unique_ptr<IDiccionarioPalabras> nextDictionary(CrearDiccionarioDesdeArchivo("Datos.txt"));
        auto next = std::make_unique<Tablero>(nextDictionary.get());
        next->llenarBolsa();
        for (int i = 0; i < count; ++i) {
            std::string name = names[i] ? names[i] : "";
            if (name.empty()) name = "Jugador " + std::to_string(i + 1);
            if (name.size() > 128) return result(false, "El nombre es demasiado largo.");
            if (next->getJugadores()->buscarJugador(name))
                return result(false, "Los nombres deben ser diferentes.");
            auto p = std::make_unique<Jugador>(name);
            next->repartirFichas(p.get(), Tablero::MAX_FICHAS_ATRIL);
            next->getJugadores()->ingresaJugadorFinal(p.release());
        }
        next->setNumJugadores(count);
        game.reset();
        dictionary = std::move(nextDictionary);
        game = std::move(next);
        active = game->jugadorInicial();
        passes = 0;
        finished = false;
        return result(true, "Partida iniciada.", true);
    });
}

const char *scrabble_play(int row, int col, int vertical, const char *word)
{
    return guard([&] {
        if (!running()) return result(false, "Inicie una partida.");
        std::string error;
        int points = 0;
        if (!game->jugarPalabra(row, col, word ? word : "", vertical ? 'V' : 'H',
                               player(), &error, &points))
            return result(false, error);
        passes = 0;
        advance();
        return result(true, "Palabra aceptada: +" + std::to_string(points) + " puntos.", true);
    });
}

const char *scrabble_exchange(const char *letters)
{
    return guard([&] {
        if (!running()) return result(false, "Inicie una partida.");
        std::string error;
        if (!game->intercambiarFichas(player(), letters ? letters : "", &error))
            return result(false, error);
        passes = 0;
        advance();
        return result(true, "Fichas intercambiadas.", true);
    });
}

const char *scrabble_pass()
{
    return guard([] {
        if (!running()) return result(false, "Inicie una partida.");
        ++passes;
        advance();
        return result(true, "Turno pasado.", true);
    });
}

const char *scrabble_finish()
{
    return guard([] {
        if (!running()) return result(false, "No hay una partida activa.");
        finished = true;
        return result(true, "Partida finalizada.");
    });
}
} // extern "C"
