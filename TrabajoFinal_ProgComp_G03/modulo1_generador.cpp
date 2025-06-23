// ------------------------------
// modulo1_generador.cpp
// ------------------------------

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>        // marca temporal dinámica
#include <windows.h>    // SetConsoleOutputCP
#include <random>
#include <algorithm>    // std::shuffle
#include <fstream>
#include "modulo1_generador.h"

using namespace std;

// Colores ANSI
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define CYAN    "\033[36m"
#define YELLOW  "\033[33m"
#define RESET   "\033[0m"

// Genera un carácter aleatorio entre 'inicio' y 'fin'
char obtenerCaracterAleatorio(char inicio, char fin) {
    return static_cast<char>(inicio + rand() % (fin - inicio + 1));
}

void generarContrasena() {
    SetConsoleOutputCP(CP_UTF8);
    system("cls");

    // Cabecera del módulo
    cout << CYAN
        << "[INFO] ===== MÓDULO 1 – Generador de Contraseñas =====\n"
        << RESET;

    // Inicialización de semilla de aleatoriedad (solo una vez)
    static bool semillaYaInicializada = false;
    if (!semillaYaInicializada) {
        srand(static_cast<unsigned int>(time(nullptr)));
        semillaYaInicializada = true;
    }

    // Solicitar longitud deseada
    int passwordLength = 0;
    string userInput;
    while (true) {
        cout << YELLOW
            << "[INPUT] Ingrese la longitud de la contraseña (mínimo 8) o 0 para volver: "
            << RESET;
        getline(cin, userInput);

        if (userInput == "0") {
            cout << CYAN << "[INFO] Regresando al menú principal...\n" << RESET;
            return;
        }

        // Validar que el usuario solo introduzca dígitos
        bool inputIsNumeric = !userInput.empty() && all_of(userInput.begin(), userInput.end(), ::isdigit);
        if (!inputIsNumeric) {
            cout << RED
                << "[ERROR] Entrada inválida: por favor utilice solo dígitos.\n"
                << RESET;
            continue;
        }

        passwordLength = stoi(userInput);
        if (passwordLength < 8) {
            cout << RED
                << "[ERROR] Longitud insuficiente: el valor mínimo es 8.\n"
                << RESET;
            continue;
        }
        break;
    }

    cout << CYAN
        << "[INFO] Generando contraseña de " << passwordLength << " caracteres...\n"
        << RESET;

    // Definición de conjuntos de caracteres
    const string uppercaseLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const string lowercaseLetters = "abcdefghijklmnopqrstuvwxyz";
    const string digitCharacters = "0123456789";
    const string symbolCharacters = "!@#$%^&*()";

    // Construcción de contraseña
    string generatedPassword;
    generatedPassword.reserve(passwordLength);

    // Asegurar al menos un carácter de cada tipo
    generatedPassword += obtenerCaracterAleatorio('A', 'Z');
    generatedPassword += obtenerCaracterAleatorio('a', 'z');
    generatedPassword += obtenerCaracterAleatorio('0', '9');
    generatedPassword += symbolCharacters[rand() % symbolCharacters.size()];

    // Rellenar el resto con mezcla de caracteres
    for (int i = 4; i < passwordLength; ++i) {
        switch (rand() % 4) {
        case 0:
            generatedPassword += obtenerCaracterAleatorio('A', 'Z');
            break;
        case 1:
            generatedPassword += obtenerCaracterAleatorio('a', 'z');
            break;
        case 2:
            generatedPassword += obtenerCaracterAleatorio('0', '9');
            break;
        default:
            generatedPassword += symbolCharacters[rand() % symbolCharacters.size()];
            break;
        }
    }

    // Desordenar la contraseña para evitar patrones
    random_device rd;
    mt19937 generator(rd());
    shuffle(generatedPassword.begin(), generatedPassword.end(), generator);

    // Mostrar contraseña generada
    cout << GREEN
        << "[SUCCESS] Su nueva contraseña es: " << generatedPassword << "\n"
        << RESET;

    // Preguntar si desea guardar en archivo
    cout << YELLOW
        << "[INPUT] ¿Desea guardar esta contraseña en un archivo? (S/N): "
        << RESET;
    string saveOption;
    getline(cin, saveOption);

    if (saveOption == "S" || saveOption == "s") {
        cout << YELLOW
            << "[INPUT] Ingrese una descripción o propósito de esta contraseña: "
            << RESET;
        string passwordDescription;
        getline(cin, passwordDescription);

        string fileMode;
        while (true) {
            cout << YELLOW
                << "[INPUT] Escriba 'S' para sobrescribir o 'A' para añadir al archivo: "
                << RESET;
            getline(cin, fileMode);
            if (fileMode == "S" || fileMode == "s" || fileMode == "A" || fileMode == "a") {
                break;
            }
            cout << RED
                << "[ERROR] Opción no válida: ingrese 'S' o 'A'.\n"
                << RESET;
        }

        const string outputFileName = "contrasenas_guardadas.txt";
        ofstream outputFile;
        if (fileMode == "S" || fileMode == "s") {
            outputFile.open(outputFileName, ios::out);
        }
        else {
            outputFile.open(outputFileName, ios::app);
        }

        if (outputFile.is_open()) {
            // Obtener timestamp en tiempo de ejecución
            time_t currentTime = time(nullptr);
            struct tm localTime;
            localtime_s(&localTime, &currentTime);
            char timestampBuffer[20];
            strftime(timestampBuffer, sizeof(timestampBuffer), "%Y-%m-%d %H:%M:%S", &localTime);

            // Escribir al archivo con delimitadores
            outputFile << "------------------------------\n"
                << "Descripción : " << passwordDescription << "\n"
                << "Contraseña  : " << generatedPassword << "\n"
                << "Fecha       : " << timestampBuffer << "\n"
                << "------------------------------\n";
            outputFile.close();

            cout << GREEN
                << "[SUCCESS] Contraseña guardada correctamente en '"
                << outputFileName << "'.\n"
                << RESET;
        }
        else {
            cout << RED
                << "[ERROR] No se pudo abrir el archivo para guardar.\n"
                << RESET;
        }
    }

    // Pausa antes de regresar al menú
    cout << CYAN
        << "[INFO] Presione Enter para volver al menú principal...\n"
        << RESET;
    getline(cin, userInput);
}
