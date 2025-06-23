// ------------------------------
// modulo2_vulnerabilidades.cpp
// ------------------------------

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <filesystem>             // std::filesystem
#include <windows.h>              // SetConsoleOutputCP(CP_UTF8)
#include "modulo2_vulnerabilidades.h"
#include "trie.h"                 // Definición de Trie y métodos

using namespace std;
namespace fs = std::filesystem;  // alias claro

// Colores ANSI para salida
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define CYAN    "\033[36m"
#define RESET   "\033[0m"

/*
 * Lee todas las líneas de 'filePath' y las inserta en 'trieStruct', evitando duplicados.
 * Retorna true si se agregaron contraseñas nuevas; false en caso contrario.
 */
bool cargarContrasenasExpuestas(const string& filePath, Trie& trieStruct) {
    ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        cerr << RED
            << "[ERROR] No se pudo abrir el archivo de contraseñas expuestas: "
            << filePath << RESET << endl;
        return false;
    }

    string line;
    int newPasswordsCount = 0;

    while (getline(inputFile, line)) {
        if (line.empty()) {
            continue;
        }
        // Insertar solo si no existe ya en el Trie
        if (!trieStruct.buscar(line)) {
            trieStruct.insertar(line);
            ++newPasswordsCount;
        }
    }

    inputFile.close();

    cout << GREEN
        << "[INFO] Se cargaron " << newPasswordsCount
        << " contraseñas nuevas desde \"" << filePath << "\".\n"
        << RESET;

    return (newPasswordsCount > 0);
}

/*
 * Solicita confirmación al usuario: 'S' para repetir, 'N' para no.
 * Retorna true si desea repetir; false en cualquier otro caso.
 */
bool obtenerConfirmacionUsuario() {
    string userInput;
    while (true) {
        getline(cin, userInput);

        // Recortar espacios
        size_t startPos = userInput.find_first_not_of(" \t\r\n");
        if (startPos == string::npos) {
            // Cadena vacía => interpretar como "no"
            return false;
        }
        size_t endPos = userInput.find_last_not_of(" \t\r\n");
        string trimmed = userInput.substr(startPos, endPos - startPos + 1);

        if (trimmed.empty()) {
            return false;
        }

        char answer = tolower(static_cast<unsigned char>(trimmed[0]));
        if (answer == 's') {
            return true;
        }
        else if (answer == 'n') {
            return false;
        }
        else {
            cout << RED
                << "[ERROR] Entrada inválida. Ingrese 'S' (sí) o 'N' (no):\n"
                << RESET
                << "[INPUT] ¿Desea verificar otra contraseña? (S/N): ";
        }
    }
}

/*
 * Módulo interactivo para verificar vulnerabilidades en contraseñas.
 * Retorna true si el usuario elige '0' para volver al menú principal.
 */
bool verificarContrasena(Trie& trieStruct) {
    string passwordToCheck;
    bool repetir;

    do {
        // 1) Solicitar contraseña
        cout << "\n[INPUT] Ingrese la contraseña a verificar (o 0 para volver): ";
        getline(cin, passwordToCheck);

        if (passwordToCheck == "0") {
            return true;  // volver al menú
        }

        if (passwordToCheck.empty()) {
            cout << RED
                << "[ERROR] La contraseña no puede estar vacía.\n"
                << RESET;
            repetir = false;
            continue;
        }

        // 2) Calcular subcadena filtrada más larga
        size_t longestFilteredSubstrLength = trieStruct.longestSubcadena(passwordToCheck);

        // 3) Definir umbral = mitad de longitud redondeada hacia arriba
        size_t vulnerabilityThreshold = (passwordToCheck.size() + 1) / 2;

        // 4) Mostrar resultado con mensajes claros
        if (longestFilteredSubstrLength == passwordToCheck.size()) {
            cout << RED
                << "[VULNERABLE] La contraseña \""
                << passwordToCheck
                << "\" está completamente expuesta.\n"
                << RESET;
        }
        else if (longestFilteredSubstrLength >= vulnerabilityThreshold) {
            cout << RED
                << "[VULNERABLE] La contraseña \""
                << passwordToCheck
                << "\" es vulnerable. Subcadena filtrada de longitud "
                << longestFilteredSubstrLength
                << " (>= " << vulnerabilityThreshold << ").\n"
                << RESET;
        }
        else if (longestFilteredSubstrLength == 0) {
            cout << GREEN
                << "[SEGURA] La contraseña \""
                << passwordToCheck
                << "\" NO contiene subcadenas filtradas.\n"
                << RESET;
        }
        else {
            cout << GREEN
                << "[SEGURA] La contraseña \""
                << passwordToCheck
                << "\" NO supera el umbral (subcadena máxima = "
                << longestFilteredSubstrLength
                << " < " << vulnerabilityThreshold << ").\n"
                << RESET;
        }

        // 5) Preguntar si desea repetir
        cout << "\n[INPUT] ¿Desea verificar otra contraseña? (S/N): ";
        repetir = obtenerConfirmacionUsuario();

    } while (repetir);

    return false;
}

/*
 * Punto de entrada del Módulo 2:
 * - Configura UTF-8
 * - Limpia pantalla y muestra cabecera
 * - Carga archivos .txt de "passwords/" solo la primera vez
 * - Llama a verificarContrasena(...) una sola vez
 */
void iniciarModulo2() {
    // Configurar consola en UTF-8
    SetConsoleOutputCP(CP_UTF8);

    // Limpiar pantalla y cabecera
    system("cls");
    cout << CYAN
        << "\n========== MÓDULO 2 – Verificar Vulnerabilidades ==========\n"
        << RESET;

    static Trie trieStruct;
    static bool archivosYaCargados = false;

    if (!archivosYaCargados) {
        const string passwordsDirectory = "passwords";
        if (!fs::is_directory(passwordsDirectory)) {
            cerr << RED
                << "[ERROR] No existe la carpeta: \"" << passwordsDirectory << "\".\n"
                << "Cree la carpeta \"passwords\" junto al ejecutable con archivos .txt dentro."
                << RESET << endl;
            cout << "\n[INFO] Presione Enter para volver al menú principal...";
            string dummy; getline(cin, dummy);
            return;
        }

        int totalTextFiles = 0;
        for (const auto& directoryEntry : fs::directory_iterator(passwordsDirectory)) {
            if (!directoryEntry.is_regular_file()) {
                continue;
            }
            if (directoryEntry.path().extension() == ".txt") {
                ++totalTextFiles;
                string filePath = directoryEntry.path().string();
                cargarContrasenasExpuestas(filePath, trieStruct);
            }
        }

        if (totalTextFiles == 0) {
            cerr << RED
                << "[ERROR] No se encontraron archivos .txt en \""
                << passwordsDirectory << "\".\n"
                << RESET;
            cout << "\n[INFO] Presione Enter para volver al menú principal...";
            string dummy; getline(cin, dummy);
            return;
        }

        archivosYaCargados = true;
    }
    else {
        cout << GREEN
            << "[INFO] Archivos ya fueron cargados previamente. Saltando carga.\n"
            << RESET;
    }

    // Ejecutar verificación de contraseñas
    bool volverAlMenu = verificarContrasena(trieStruct);
    (void)volverAlMenu;  // su valor indica regreso, pero siempre retornamos
}
