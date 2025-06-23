#include <iostream>
#include <limits>
#include <cstdlib>
#include <windows.h>      // Para SetConsoleOutputCP (UTF-8 en Windows)

#include "modulo1_generador.h"           // M�dulo 1
#include "trie.h"                        // Estructura Trie
#include "modulo2_vulnerabilidades.h"    // M�dulo 2
#include "modulo3_monitor.h"

using namespace std;

// Colores ANSI
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define CYAN    "\033[36m"
#define RESET   "\033[0m"

// Prototipado de funciones del men�
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

    cout << GREEN << "\nGracias por usar el sistema de seguridad inform�tica. �Hasta luego!\n" << RESET;
    return 0;
}

void mostrarMenu() {
    system("cls");  // Limpiar consola en Windows

    cout << CYAN;
    cout << "\n================= SecureNet Solutions =================\n";
    cout << "1. Generar contrase�a segura\n";
    cout << "2. Analizar vulnerabilidades en contrase�as\n";
    cout << "3. Monitorear procesos\n";
    cout << "0. Salir\n";
    cout << "=======================================================\n";
    cout << RESET;
    cout << "Seleccione una opci�n: ";
}

int obtenerOpcion() {
    string entrada;
    int opcion;

    while (true) {
        getline(cin, entrada);

        // Validar que la entrada sea num�rica
        bool esNumerico = true;
        for (char c : entrada) {
            if (!isdigit(c)) { esNumerico = false; break; }
        }

        if (!esNumerico || entrada.empty()) {
            cout << RED << "[Error] Entrada inv�lida. Solo n�meros del 0 al 3.\n" << RESET;
            cout << "Seleccione una opci�n: ";
            continue;
        }

        opcion = stoi(entrada);
        if (opcion < 0 || opcion > 3) {
            cout << RED << "[Error] Opci�n fuera de rango. Ingrese un n�mero entre 0 y 3.\n" << RESET;
            cout << "Seleccione una opci�n: ";
        }
        else {
            return opcion;
        }
    }
}

void ejecutarOpcion(int opcion) {
    switch (opcion) {
    case 1:
        // M�dulo 1: Generar contrase�a
        generarContrasena();
        break;

    case 2:
        // M�dulo 2: Verificar vulnerabilidades (substrings)
        iniciarModulo2();
        break;

    case 3:
        // M�dulo 
		monitorearProcesos();
        break;

    case 0:
        // Salir
        break;

    default:
        // No deber�a ocurrir
        break;
    }
}
