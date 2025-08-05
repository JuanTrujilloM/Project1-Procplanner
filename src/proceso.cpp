#include "proceso.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
using namespace std;

std::vector<Proceso> cargarDesdeArchivo(const std::string& nombreArchivo) {
    std::vector<Proceso> procesos;
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "Error: No se pudo abrir el archivo " << nombreArchivo << endl;
        return procesos;
    }
    string linea;
    while (getline(archivo, linea)) {
        if (linea.find("PID:") != string::npos) {
            Proceso p;
            p.pc = 0;
            p.ax = 0; // Inicializa AX en 0
            p.bx = 0; // Inicializa BX en 0
            p.cx = 0; // Inicializa CX en 0
            strcpy(p.estado, "Listo");
            istringstream ss(linea);
            string temp;
            ss >> temp >> p.pid;
            bool ax_set = false, bx_set = false, cx_set = false;
            while (ss >> temp) {
                if (temp.find("AX=") == 0) {
                    p.ax = stoi(temp.substr(3));
                    ax_set = true;
                } else if (temp.find("BX=") == 0) {
                    p.bx = stoi(temp.substr(3));
                    bx_set = true;
                } else if (temp.find("CX=") == 0) {
                    p.cx = stoi(temp.substr(3));
                    cx_set = true;
                } else if (temp.find("Quantum=") == 0) {
                    p.quantum = stoi(temp.substr(8));
                } else if (temp.find("Q=") == 0) {
                    p.quantum = stoi(temp.substr(2));
                }
            }
            // Si no aparecen AX, BX o CX, ya están en 0
            procesos.push_back(p);
        }
    }
    archivo.close();
    return procesos;
}

void mostrarProcesos(const std::vector<Proceso>& procesos) {
    cout << "PID\tPC\tAX\tBX\tCX\tQuantum\tEstado" << endl;
    cout << "---\t--\t--\t--\t--\t-------\t------" << endl;
    for (const auto& p : procesos) {
        cout << p.pid << "\t" << p.pc << "\t" << p.ax << "\t"
             << p.bx << "\t" << p.cx << "\t" << p.quantum << "\t"
             << p.estado << endl;
    }
}

void ejecutarProcesos(std::vector<Proceso>& procesos) {
    cout << "\n=== INICIO DE PLANIFICACIÓN ROUND ROBIN ===" << endl;
    for (size_t i = 0; i < procesos.size(); ++i) {
        Proceso& p = procesos[i];
        if (i > 0) {
            Proceso& anterior = procesos[i - 1];
            cout << "\n[Cambio de contexto]" << endl;
            cout << "Guardando estado de Proceso " << anterior.pid
                 << ": PC=" << anterior.pc
                 << ", AX=" << anterior.ax
                 << ", BX=" << anterior.bx << endl;
            cout << "Cargando estado de Proceso " << p.pid
                 << ": PC=" << p.pc
                 << ", AX=" << p.ax
                 << ", BX=" << p.bx << endl << endl;
        }
        strcpy(p.estado, "Ejecutando");
        cout << ">> Ejecutando PID " << p.pid << " durante " << p.quantum << " ciclos o hasta que no hayan mas instrucciones." << endl;
        int ciclosEjecutados = 0;
        
        p.pc++;
        ciclosEjecutados++;
        p.quantum--;
        cout << "   Ciclo " << ciclosEjecutados << " | PC = " << p.pc << " | Quantum restante = " << p.quantum << " | Inst: " << inst << endl;
        mostrarProcesos(procesos);
        cout << "\n";

        if (p.pc >= (int)instrucciones.size()) {
            cout << "Proceso " << p.pid << " ha terminado todas sus instrucciones." << endl;
            strcpy(p.estado, "Terminado");
            mostrarProcesos(procesos);
        } else if (p.quantum == 0) {
            cout << "Proceso " << p.pid << " ha agotado su quantum." << endl;
            strcpy(p.estado, "Terminado");
            mostrarProcesos(procesos);
        }
    }
    cout << "\n=== FIN DE LA PLANIFICACIÓN ===\n" << endl;
}
