#include <iostream>
#include <limits>
#include <cstdlib>
#include <windows.h>      // Para SetConsoleOutputCP (UTF-8 en Windows)

#include "modulo1_generador.h"           // Módulo 1
#include "trie.h"                        // Estructura Trie
#include "modulo2_vulnerabilidades.h"    // Módulo 2
#include "modulo3_monitor.h"

using namespace std;

// Colores ANSI
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define CYAN    "\033[36m"
#define RESET   "\033[0m"

// Prototipado de funciones del menú
void mostrarMenu();
int obtenerOpcion();
void ejecutarOpcion(int opcion);

int main() {
    // Configurar la consola para UTF-8 en Windows
    SetConsoleOutputCP(CP_UTF8);
    int opcion;
    do {
        mostrarMenu();
        opcion = obtenerOpcion();
        ejecutarOpcion(opcion);
    } while (opcion != 0);

    cout << GREEN << "\nGracias por usar el sistema de seguridad informática. ¡Hasta luego!\n" << RESET;
    return 0;
}

void mostrarMenu() {
    system("cls");  // Limpiar consola en Windows

    cout << CYAN;
    cout << "\n================= SecureNet Solutions =================\n";
    cout << "1. Generar contraseña segura\n";
    cout << "2. Analizar vulnerabilidades en contraseñas\n";
    cout << "3. Monitorear procesos\n";
    cout << "0. Salir\n";
    cout << "=======================================================\n";
    cout << RESET;
    cout << "Seleccione una opción: ";
}

int obtenerOpcion() {
    string entrada;
    int opcion;

    while (true) {
        getline(cin, entrada);

        // Validar que la entrada sea numérica
        bool esNumerico = true;
        for (char c : entrada) {
            if (!isdigit(c)) { esNumerico = false; break; }
        }

        if (!esNumerico || entrada.empty()) {
            cout << RED << "[Error] Entrada inválida. Solo números del 0 al 3.\n" << RESET;
            cout << "Seleccione una opción: ";
            continue;
        }

        opcion = stoi(entrada);
        if (opcion < 0 || opcion > 3) {
            cout << RED << "[Error] Opción fuera de rango. Ingrese un número entre 0 y 3.\n" << RESET;
            cout << "Seleccione una opción: ";
        }
        else {
            return opcion;
        }
    }
}

void ejecutarOpcion(int opcion) {
    switch (opcion) {
    case 1:
        // Módulo 1: Generar contraseña
        generarContrasena();
        break;

    case 2:
        // Módulo 2: Verificar vulnerabilidades (substrings)
        iniciarModulo2();
        break;

    case 3:
        // Módulo 
		monitorearProcesos();
        break;

    case 0:
        // Salir
        break;

    default:
        // No debería ocurrir
        break;
    }
}
