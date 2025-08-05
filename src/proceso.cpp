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
        // Leer instrucciones del archivo correspondiente al proceso
        string nombreArchivo = "input/instrucciones/" + to_string(p.pid) + ".txt";
        vector<string> instrucciones;
        ifstream archivoInst(nombreArchivo);
        if (archivoInst.is_open()) {
            string lineaInst;
            while (getline(archivoInst, lineaInst)) {
                instrucciones.push_back(lineaInst);
            }
            archivoInst.close();
        } else {
            cerr << "No se pudo abrir el archivo de instrucciones para PID " << p.pid << endl;
        }

        while (p.quantum > 0 && p.pc < (int)instrucciones.size()) {
            string inst = instrucciones[p.pc];
            // Eliminar espacios y comas extra
            for (auto& c : inst) if (c == ',') c = ' ';
            istringstream iss(inst);
            string op;
            iss >> op;
            if (op == "INC") {
                string reg;
                iss >> reg;
                if (reg == "AX") p.ax++;
                else if (reg == "BX") p.bx++;
                else if (reg == "CX") p.cx++;
            } else if (op == "ADD" || op == "SUB" || op == "MUL") {
                string reg1, reg2;
                iss >> reg1 >> reg2;
                int *dest = nullptr;
                if (reg1 == "AX") dest = &p.ax;
                else if (reg1 == "BX") dest = &p.bx;
                else if (reg1 == "CX") dest = &p.cx;

                if (dest) {
                    int val = 0;
                    if (reg2 == "AX") val = p.ax;
                    else if (reg2 == "BX") val = p.bx;
                    else if (reg2 == "CX") val = p.cx;
                    else {
                        val = stoi(reg2);
                    }
                    if (op == "ADD") *dest += val;
                    else if (op == "SUB") *dest -= val;
                    else if (op == "MUL") *dest *= val;
                }
            } else if (op == "NOP") {
                // No hacer nada
            } else if (op == "JMP") {
                int destino;
                iss >> destino;
                if (destino >= 0 && destino < (int)instrucciones.size()) {
                    p.pc = destino - 1; // -1 porque se incrementa abajo
                }
            }
            p.pc++;
            ciclosEjecutados++;
            p.quantum--;
            cout << "   Ciclo " << ciclosEjecutados << " | PC = " << p.pc << " | Quantum restante = " << p.quantum << " | Inst: " << inst << endl;
            mostrarProcesos(procesos);
            cout << "\n";
        }

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
