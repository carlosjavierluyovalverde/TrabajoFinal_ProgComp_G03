#pragma once
#ifndef MODULO2_VULNERABILIDADES_H_INCLUDED
#define MODULO2_VULNERABILIDADES_H_INCLUDED

#include <string>
#include "trie.h"

using namespace std;

// Carga todas las contraseñas expuestas desde un archivo al Trie.
// Retorna true si al menos se cargó una contraseña en alguno de los .txt, false en caso contrario.
bool cargarContrasenasExpuestas(const string& nombreArchivo, Trie& trie);

// Verifica si alguna subcadena de la contraseña ingresada está en el Trie.
// - Si el usuario ingresa "0", retorna true para volver al menú principal.
// - De lo contrario, informa si contiene subcadenas vulnerables y pregunta si desea verificar otra.
bool verificarContrasena(Trie& trie);

// Punto de entrada para ejecutar TODO el flujo de Módulo 2:
//  - Buscará automáticamente todos los .txt en la carpeta "passwords"
//  - Construirá el Trie la primera vez que se invoque
//  - Luego ejecutará el bucle de verificación interactiva
void iniciarModulo2();

#endif // MODULO2_VULNERABILIDADES_H_INCLUDED
