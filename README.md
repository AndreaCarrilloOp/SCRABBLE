<div align="center">

# SCRABBLE en C++

**Juego de consola multijugador desarrollado como proyecto académico de Estructuras de Datos.**

![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-3.16%2B-064F8C?style=for-the-badge&logo=cmake&logoColor=white)
![Visual Studio](https://img.shields.io/badge/Visual%20Studio-2022-5C2D91?style=for-the-badge&logo=visualstudio&logoColor=white)
![Tests](https://img.shields.io/badge/Tests-CTest-success?style=for-the-badge&logo=checkmarx&logoColor=white)
![Plataformas](https://img.shields.io/badge/Plataformas-Windows%20%7C%20Linux-informational?style=for-the-badge)
![Estado](https://img.shields.io/badge/Estado-Estable-brightgreen?style=for-the-badge)

</div>

---

## Descripción

SCRABBLE es una implementación en **C++17** del juego de palabras para consola. Permite partidas de **2 a 4 jugadores** sobre un tablero de **15 × 15**, con atriles de seis fichas, validación mediante un diccionario local, cálculo de puntuaciones, bonificaciones del tablero e historial de jugadas.

El proyecto aplica programación orientada a objetos, estructuras enlazadas personalizadas, manejo de archivos, validación de entradas y pruebas automatizadas.

## Características principales

- Partidas para **2 a 4 jugadores**.
- Tablero clásico de **15 × 15 casillas** con cuadrícula verde y casillas de bonificación a color.
- Atril de **6 fichas por jugador**.
- Selección aleatoria del jugador inicial.
- Colocación horizontal o vertical de palabras.
- Validación de palabras mediante `Datos.txt`.
- Comprobación de límites, conexiones y letras superpuestas.
- Validación de las palabras cruzadas generadas por una jugada.
- Cálculo de puntos y aplicación de bonificaciones.
- Intercambio de fichas con la bolsa.
- Historial de palabras aceptadas.
- Control de pases consecutivos y finalización de la partida.
- Clasificación final de participantes por puntaje.
- Manejo seguro de entradas inválidas en consola.
- Pruebas automatizadas integradas con **CTest**.

## Tecnologías utilizadas

| Tecnología | Uso |
|---|---|
| C++17 | Implementación principal |
| CMake 3.16 o superior | Configuración y compilación multiplataforma |
| Visual Studio 2022 | Solución nativa para Windows |
| CTest | Ejecución de pruebas automatizadas |
| Biblioteca estándar de C++ | Archivos, cadenas, consola y utilidades |

El proyecto no requiere bibliotecas externas.

## Estructura del proyecto

```text
SCRABBLE/
├── include/                 # Encabezados y contratos de las clases
├── src/                     # Implementación del juego
├── test/                    # Pruebas automatizadas
├── Datos.txt                # Diccionario local de palabras
├── CMakeLists.txt           # Configuración de CMake y CTest
├── SCRABBLE.sln             # Solución de Visual Studio
├── SCRABBLE.vcxproj         # Proyecto de Visual Studio
├── compilar.bat             # Compilación automatizada en Windows
└── README.md                # Documentación principal
```

## Requisitos

### Opción 1: Visual Studio

- Windows 10 u 11.
- Visual Studio 2022 o posterior.
- Componente **Desarrollo para el escritorio con C++**.
- Toolset de C++ predeterminado de la instalación de Visual Studio (compatible con C++17).

### Opción 2: CMake

- Compilador compatible con C++17, por ejemplo:
  - Microsoft Visual C++.
  - GCC.
  - Clang.
- CMake 3.16 o superior.

## Compilar con Visual Studio

1. Abra `SCRABBLE.sln`.
2. Seleccione la plataforma `x64`.
3. Seleccione la configuración `Debug` o `Release`.
4. Ejecute **Build > Build Solution**.
5. Inicie el proyecto desde Visual Studio.

El archivo `Datos.txt` se copia automáticamente junto al ejecutable.

> [!NOTE]
> El proyecto utiliza C++17 y el toolset predeterminado de la instalación (por ejemplo, `v143` en Visual Studio 2022). Si existen binarios antiguos, cierre el juego y ejecute **Build > Clean Solution** y **Build > Rebuild Solution**. No ejecute la última versión compilada si la nueva compilación falla.

## Compilar con CMake

Desde la carpeta raíz del proyecto:

```bash
cmake -S . -B build
cmake --build build --config Release
```

### Ejecutar en Windows

```powershell
.\build\Release\SCRABBLE.exe
```

Según el generador utilizado, el ejecutable también podría encontrarse directamente en `build`.

### Ejecutar en Linux

```bash
./build/SCRABBLE
```

## Vista previa del tablero

En terminales compatibles con secuencias ANSI se muestran las coordenadas de 0 a 14 y las casillas de bonificación:

| Color | Bonificación |
|---|---|
| Rojo | Triple palabra |
| Magenta | Doble palabra |
| Azul | Triple letra |
| Celeste | Doble letra |
| Amarillo | Centro (doble palabra) |

Cada casilla ocupa cinco caracteres de ancho. Se recomienda una terminal de al menos 100 columnas para evitar saltos de línea.

Las fichas colocadas tienen un fondo claro. Una consola sin soporte de color o con la variable `NO_COLOR` conserva los códigos de bonificación en texto.

Vista previa sin iniciar una partida:

```powershell
.\build\Release\SCRABBLE.exe --preview
```

```bash
./build/SCRABBLE --preview
```

Use `--preview-plain` para forzar texto o `--preview-color` para generar explícitamente la salida ANSI.

## Compilar automáticamente en Windows

También puede utilizar el script incluido:

```bat
compilar.bat
```

Este script configura el proyecto, compila la solución y ejecuta las pruebas.

## Ejecutar las pruebas

```bash
cmake -S . -B build
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

Las pruebas incluidas verifican jugadas válidas e inválidas, puntuación de palabras extendidas, intercambio de fichas, integridad de la lista enlazada y la distribución de las bonificaciones. Las comprobaciones se ejecutan también en compilaciones Release.

## Flujo de una partida

1. El sistema carga el diccionario desde `Datos.txt`.
2. Se registra la cantidad de participantes y sus nombres.
3. Se llena la bolsa y se reparten seis fichas a cada jugador.
4. Se selecciona aleatoriamente al jugador inicial.
5. En cada turno, el participante puede:
   - Jugar una palabra.
   - Cambiar fichas.
   - Consultar el historial.
   - Pasar el turno.
   - Finalizar la partida.
6. Al terminar, el sistema aplica los ajustes correspondientes y muestra la clasificación final.

## Reglas implementadas

- Las palabras deben contener entre **2 y 6 letras**.
- Todas las palabras deben existir en `Datos.txt`.
- La primera palabra debe cruzar la casilla central `(7,7)`.
- Las jugadas posteriores deben conectarse con fichas existentes.
- Las letras colocadas sobre fichas existentes deben coincidir.
- La palabra debe caber completamente dentro del tablero.
- Las palabras cruzadas creadas por una jugada también deben ser válidas.
- Las bonificaciones de letra y palabra se aplican únicamente a fichas nuevas.
- Las fichas utilizadas se retiran del atril y se reponen desde la bolsa.
- Una jugada inválida no consume el turno.
- La partida puede finalizar cuando:
  - Se acumulan dos rondas completas de pases consecutivos.
  - La bolsa queda vacía y un jugador se queda sin fichas.
  - Los participantes deciden terminarla manualmente.

## Diccionario de palabras

El archivo `Datos.txt` contiene las palabras admitidas por el juego. Debe permanecer junto al ejecutable o en una de las rutas de búsqueda previstas por la aplicación.

Para ampliar el diccionario, agregue una palabra por línea respetando las reglas del proyecto. Se recomienda utilizar letras sin espacios ni caracteres especiales.

## Diseño del sistema

Entre las clases principales se encuentran:

| Clase | Responsabilidad |
|---|---|
| `GestorJuego` | Coordina la inicialización, los turnos y el flujo general |
| `Tablero` | Gestiona la cuadrícula, las jugadas, la puntuación y la bolsa |
| `Jugador` | Mantiene el nombre, puntaje y atril de cada participante |
| `Ficha` | Representa una letra y su valor |
| `ListaFicha` | Administra las colecciones de fichas |
| `ListaJugador` | Administra y ordena los participantes |
| `ValidadorPalabras` | Consulta la validez de palabras |
| `HistorialPalabras` | Registra las palabras aceptadas |
| `Consola` | Gestiona la presentación y configuración de la terminal |


## Uso académico

Este repositorio fue desarrollado con fines educativos para aplicar conceptos de:

- Programación orientada a objetos.
- Estructuras de datos.
- Manejo dinámico de memoria.
- Lectura de archivos.
- Modularización de software.
- Validación de reglas de negocio.
- Pruebas de software.

---

<div align="center">

**Desarrollado en C++ como proyecto académico de Estructuras de Datos.**

</div>
