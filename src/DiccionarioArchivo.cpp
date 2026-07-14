#include "FabricaDiccionarioPalabras.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <stdexcept>
#include <unordered_set>

namespace
{
std::string normalizar(std::string palabra)
{
    palabra.erase(std::remove_if(palabra.begin(), palabra.end(), [](unsigned char c) {
        return std::isspace(c) != 0;
    }), palabra.end());
    for (char &c : palabra)
        c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    return palabra;
}

class DiccionarioArchivo final : public IDiccionarioPalabras
{
private:
    std::unordered_set<std::string> palabras;

public:
    explicit DiccionarioArchivo(const std::string &ruta)
    {
        std::ifstream archivo(ruta);
        if (!archivo)
            throw std::runtime_error("No se pudo abrir el diccionario: " + ruta);
        std::string palabra;
        while (archivo >> palabra)
        {
            palabra = normalizar(palabra);
            if (!palabra.empty())
                palabras.insert(palabra);
        }
        if (palabras.empty())
            throw std::runtime_error("El diccionario está vacío: " + ruta);
    }

    bool contiene(const std::string &palabra) const override
    {
        return palabras.find(normalizar(palabra)) != palabras.end();
    }
};
}

IDiccionarioPalabras *CrearDiccionarioDesdeArchivo(const std::string &ruta)
{
    return new DiccionarioArchivo(ruta);
}
