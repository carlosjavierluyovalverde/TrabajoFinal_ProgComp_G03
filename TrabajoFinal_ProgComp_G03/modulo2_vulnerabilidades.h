#pragma once
#ifndef MODULO2_VULNERABILIDADES_H_INCLUDED
#define MODULO2_VULNERABILIDADES_H_INCLUDED

#include <string>
#include "trie.h"

using namespace std;

// Carga todas las contrase�as expuestas desde un archivo al Trie.
// Retorna true si al menos se carg� una contrase�a en alguno de los .txt, false en caso contrario.
bool cargarContrasenasExpuestas(const string& nombreArchivo, Trie& trie);

// Verifica si alguna subcadena de la contrase�a ingresada est� en el Trie.
// - Si el usuario ingresa "0", retorna true para volver al men� principal.
// - De lo contrario, informa si contiene subcadenas vulnerables y pregunta si desea verificar otra.
bool verificarContrasena(Trie& trie);

// Punto de entrada para ejecutar TODO el flujo de M�dulo 2:
//  - Buscar� autom�ticamente todos los .txt en la carpeta "passwords"
//  - Construir� el Trie la primera vez que se invoque
//  - Luego ejecutar� el bucle de verificaci�n interactiva
void iniciarModulo2();

#endif // MODULO2_VULNERABILIDADES_H_INCLUDED
