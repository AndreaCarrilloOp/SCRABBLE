cmake_minimum_required(VERSION 3.16)
project(SCRABBLE LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SCRABBLE_CORE_SOURCES
    src/Consola.cpp
    src/Cuadricula.cpp
    src/DiccionarioArchivo.cpp
    src/Ficha.cpp
    src/GestorJuego.cpp
    src/HistorialPalabras.cpp
    src/ITablero.cpp
    src/Jugador.cpp
    src/ListaFicha.cpp
    src/ListaJugador.cpp
    src/ListaPalabra.cpp
    src/Palabra.cpp
    src/Tablero.cpp
    src/ValidadorPalabras.cpp
)

add_library(scrabble_core STATIC ${SCRABBLE_CORE_SOURCES})
target_include_directories(scrabble_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(MSVC)
    target_compile_options(scrabble_core PRIVATE /W4 /utf-8)
else()
    target_compile_options(scrabble_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(SCRABBLE src/Principal.cpp)
target_link_libraries(SCRABBLE PRIVATE scrabble_core)

add_custom_command(TARGET SCRABBLE POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/Datos.txt" "$<TARGET_FILE_DIR:SCRABBLE>/Datos.txt"
    VERBATIM)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Datos.txt
               ${CMAKE_CURRENT_BINARY_DIR}/Datos.txt COPYONLY)

include(CTest)
if(BUILD_TESTING)
    add_executable(scrabble_tests tests/native/testJuego.cpp)
    target_link_libraries(scrabble_tests PRIVATE scrabble_core)
    add_test(NAME funcionamiento_scrabble COMMAND scrabble_tests)
    add_test(NAME tablero_sin_color COMMAND SCRABBLE --preview-plain)
    add_test(NAME tablero_con_color COMMAND SCRABBLE --preview-color)
    set_tests_properties(tablero_sin_color tablero_con_color PROPERTIES
        PASS_REGULAR_EXPRESSION "TABLERO ACTUAL")
endif()
