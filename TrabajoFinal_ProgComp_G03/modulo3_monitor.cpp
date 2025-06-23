// ------------------------------
// módulo3_monitor.cpp
// ------------------------------

#ifdef UNICODE
#undef UNICODE
#endif
#ifdef _UNICODE
#undef _UNICODE
#endif

#include "modulo3_monitor.h"

#include <windows.h>
#include <tlhelp32.h>       // CreateToolhelp32Snapshot, PROCESSENTRY32
#include <psapi.h>          // GetProcessMemoryInfo
#include <conio.h>          // _kbhit(), _getch()
#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <algorithm>
#include <chrono>
#include <thread>
#include <fstream>
#include <ctime>
#include <atomic>
#include <mutex>

using namespace std;

// Colores ANSI
#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define GREEN   "\033[32m"
#define CYAN    "\033[36m"
#define RESET   "\033[0m"

// Control del hilo y visibilidad
static thread           monitorThread;
static atomic<bool>     monitorStopFlag{ false };
static atomic<bool>     monitorRunning{ false };
static atomic<bool>     monitorVisible{ false };
static size_t           monitorThresholdMB = 0;
static const size_t     TOP_N = 5;
static mutex            startMutex;

// Función de monitoreo en segundo plano
static void monitorLoop(size_t thresholdMB) {
    SIZE_T thresholdBytes = thresholdMB * 1024 * 1024;
    ofstream logFile("procesos_consumo.log", ios::app);
    if (logFile) {
        time_t t; time(&t);
        char buf[26]; ctime_s(buf, sizeof(buf), &t);
        logFile << "\n=== Monitor iniciado: " << buf
            << " Umbral=" << thresholdMB << " MB ===\n";
    }

    while (!monitorStopFlag.load()) {
        HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (snap == INVALID_HANDLE_VALUE) break;

        PROCESSENTRY32 pe{ sizeof(pe) };
        if (!Process32First(snap, &pe)) {
            CloseHandle(snap);
            break;
        }

        vector< tuple<SIZE_T, string, DWORD> > todos;

        do {
            string name = pe.szExeFile;
            DWORD pid = pe.th32ProcessID;

            HANDLE h = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                FALSE, pid);
            if (h) {
                PROCESS_MEMORY_COUNTERS pmc;
                if (GetProcessMemoryInfo(h, &pmc, sizeof(pmc))) {
                    SIZE_T uso = pmc.WorkingSetSize;
                    todos.emplace_back(uso, name, pid);

                    // Sólo imprimir en consola si el usuario está en el monitor
                    if (uso > thresholdBytes && monitorVisible.load()) {
                        cout << YELLOW
                            << "[AVISO] Alto uso RAM: " << name
                            << " (PID " << pid << ") -> "
                            << (uso / 1024 / 1024) << " MB\n"
                            << RESET;
                    }
                    // Siempre registrar en el log
                    if (uso > thresholdBytes && logFile) {
                        logFile << "[AVISO] " << name
                            << " (PID " << pid << ") "
                            << (uso / 1024 / 1024) << " MB\n";
                    }
                }
                CloseHandle(h);
            }
        } while (!monitorStopFlag.load() && Process32Next(snap, &pe));

        CloseHandle(snap);

        // Top N por consumo (sólo log)
        sort(todos.begin(), todos.end(),
            [](auto& a, auto& b) { return get<0>(a) > get<0>(b); });
        if (logFile) {
            logFile << "\n--- Top " << TOP_N << " procesos ---\n";
            for (size_t i = 0; i < min(TOP_N, todos.size()); ++i) {
                auto& [mem, nm, id] = todos[i];
                logFile << (i + 1) << ". " << nm
                    << " (PID " << id << ") "
                    << (mem / 1024 / 1024) << " MB\n";
            }
            logFile << "------------------------------\n";
        }

        this_thread::sleep_for(chrono::seconds(2));
    }

    // registro de cierre
    if (logFile) {
        time_t t; time(&t);
        char buf[26]; ctime_s(buf, sizeof(buf), &t);
        logFile << "\n=== Monitor detenido: " << buf << "===\n";
        logFile.close();
    }
    monitorRunning = false;
}

// Interfaz de usuario para iniciar/detener el monitor
void monitorearProcesos() {
    lock_guard<mutex> lk(startMutex);

    if (!monitorRunning.load()) {
        // ingresar umbral
        system("cls");
        cout << CYAN << "[INFO] MÓDULO 3 – Monitor de Procesos\n" << RESET;

        string entrada;
        size_t umbral = 0;
        while (true) {
            cout << YELLOW
                << "[INPUT] Umbral RAM en MB (0=cancelar): "
                << RESET;
            getline(cin, entrada);
            if (entrada == "0") {
                cout << CYAN << "[INFO] Cancelado.\n" << RESET;
                this_thread::sleep_for(chrono::seconds(1));
                system("cls");
                return;
            }
            bool ok = !entrada.empty() && all_of(entrada.begin(), entrada.end(), ::isdigit);
            if (!ok) {
                cout << RED << "[ERROR] Número inválido.\n" << RESET;
                continue;
            }
            umbral = stoul(entrada);
            if (umbral == 0) continue;
            break;
        }

        monitorThresholdMB = umbral;
        monitorStopFlag = false;
        monitorVisible = true;              // habilita impresiones en consola
        monitorThread = thread(monitorLoop, monitorThresholdMB);
        monitorRunning = true;

        system("cls");
        cout << CYAN
            << "[INFO] Monitor activo (umbral: " << monitorThresholdMB << " MB)\n"
            << "[INFO] Pulse 'q' para detener.\n"
            << "[INFO] Otra tecla sale al menú (monitor sigue en background).\n"
            << RESET;
    }
    else {
		system("cls");
        monitorVisible = true;                 // reactivar impresiones en consola
        cout << CYAN
            << "[INFO] Monitor ya activo (umbral: " << monitorThresholdMB << " MB)\n"
            << "[INFO] 'q' detiene.\n"
            << "[INFO] Otra tecla sale al menú.\n"
            << RESET;
    }

    // esperar pulsación del usuario
    while (true) {
        if (_kbhit()) {
            char c = _getch();
            if (c == 'q' || c == 'Q') {
                monitorVisible = false;          // desactivar impresiones
                monitorStopFlag = true;
                if (monitorThread.joinable())
                    monitorThread.join();
                cout << GREEN << "[INFO] Monitor detenido.\n" << RESET;
            }
            else {
                monitorVisible = false;          // salir sin detener hilo
                cout << CYAN << "[INFO] Saliendo al menú.\n" << RESET;
            }
            this_thread::sleep_for(chrono::seconds(1));
            system("cls");
            break;
        }
    }
}
