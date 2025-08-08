#include "proceso.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <unordered_map>
#include <vector>
#include <algorithm>
using namespace std;

// Se declara la funcion para cargar los procesos desde un archivo
std::unordered_map<int, Proceso> cargarDesdeArchivo(const std::string& nombreArchivo) {
    std::unordered_map<int, Proceso> procesos;
    std::ifstream archivo(nombreArchivo);

    // Verifica si el archivo se abrió correctamente
    if (!archivo.is_open()) {
        cerr << "Error: No se pudo abrir el archivo " << nombreArchivo << endl;
        return procesos;
    }

    // Lee el archivo línea por línea y extrae los datos de cada proceso
    string linea;
    while (getline(archivo, linea)) {
        if (linea.find("PID:") != string::npos) {
            Proceso p;

            // Inicializa los campos en 0 y despues lee dato por dato
            p.pc = 0;
            p.ax = 0;
            p.bx = 0; 
            p.cx = 0; 
            strcpy(p.estado, "Listo");
            istringstream ss(linea);
            string temp;
            ss >> temp >> p.pid;
            while (ss >> temp) {
                if (temp.find("AX=") == 0) {
                    p.ax = stoi(temp.substr(3));
                } else if (temp.find("BX=") == 0) {
                    p.bx = stoi(temp.substr(3));
                } else if (temp.find("CX=") == 0) {
                    p.cx = stoi(temp.substr(3));
                } else if (temp.find("Quantum=") == 0) {
                    p.quantum = stoi(temp.substr(8));
                } else if (temp.find("Q=") == 0) {
                    p.quantum = stoi(temp.substr(2));
                }
            }
            procesos[p.pid] = p;
        }
    }
    archivo.close();
    return procesos;
}

// Se declara la funcion para mostrar los procesos en formato tabla ordenado
void mostrarProcesos(const std::unordered_map<int, Proceso>& procesos) {

    // Imprime la cabecera de la tabla
    cout << "PID\tPC\tAX\tBX\tCX\tQuantum\tEstado" << endl;
    cout << "---\t--\t--\t--\t--\t-------\t------" << endl;

    // Obtener los PIDs y ordenarlos
    vector<int> pids;
    for (const auto& par : procesos) {
        pids.push_back(par.first);
    }
    sort(pids.begin(), pids.end());

    // Imprimir cada proceso en formato tabla
    for (int pid : pids) {
        const Proceso& p = procesos.at(pid);
        cout << p.pid << "\t" << p.pc << "\t" << p.ax << "\t"
             << p.bx << "\t" << p.cx << "\t" << p.quantum << "\t"
             << p.estado << endl;
    }
}

// Funcion para ejecutar los procesos en un planificador Round Robin
void ejecutarProcesos(std::unordered_map<int, Proceso>& procesos) {
    cout << "\n=== INICIO DE PLANIFICACIÓN ROUND ROBIN ===" << endl;
    // Inicializar aleatoriedad para interrupciones simuladas
    srand(time(0));

    // Abrir archivo de log
    ofstream log("simulacion.log");
    if (!log.is_open()) {
        cerr << "No se pudo abrir simulacion.log para escribir el seguimiento." << endl;
    }

    // Para mantener el orden de ejecución, creamos un vector de PIDs ordenados
    vector<int> ordenPIDs;
    for (const auto& par : procesos) {
        ordenPIDs.push_back(par.first);
    }
    sort(ordenPIDs.begin(), ordenPIDs.end());

    // Ejecuta cada proceso en el orden de los PIDs
    for (size_t i = 0; i < ordenPIDs.size(); ++i) {
        int pid = ordenPIDs[i];
        Proceso& p = procesos[pid];
        if (i > 0) {
            int pidAnterior = ordenPIDs[i - 1];
            Proceso& anterior = procesos[pidAnterior];
            cout << "\n[Cambio de contexto]" << endl;
            cout << "Guardando estado de Proceso " << anterior.pid
                 << ": PC=" << anterior.pc
                 << ", AX=" << anterior.ax
                 << ", BX=" << anterior.bx << endl;
            cout << "Cargando estado de Proceso " << p.pid
                 << ": PC=" << p.pc
                 << ", AX=" << p.ax
                 << ", BX=" << p.bx << endl << endl;
            if (log.is_open()) {
                log << "[Cambio de contexto] Guardando estado de Proceso " << anterior.pid
                    << ": PC=" << anterior.pc << ", AX=" << anterior.ax << ", BX=" << anterior.bx << endl;
                log << "[Cambio de contexto] Cargando estado de Proceso " << p.pid
                    << ": PC=" << p.pc << ", AX=" << p.ax << ", BX=" << p.bx << endl;
            }
        }
        strcpy(p.estado, "Ejecutando");
        cout << ">> Ejecutando PID " << p.pid << " durante " << p.quantum << " ciclos o hasta que no hayan mas instrucciones." << endl;
        if (log.is_open()) log << ">> Ejecutando PID " << p.pid << " durante " << p.quantum << " ciclos o hasta que no hayan mas instrucciones." << endl;
        int ciclosEjecutados = 0;

        // Leer instrucciones del archivo correspondiente al proceso
        string nombreArchivo = "input/instrucciones/" + to_string(p.pid) + ".txt";

        // Verifica si el archivo de instrucciones existe y se puede abrir
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
            if (log.is_open()) log << "No se pudo abrir el archivo de instrucciones para PID " << p.pid << endl;
        }

        // Ejecuta las instrucciones del proceso hasta que se agote el quantum o no haya más instrucciones
        while (p.quantum > 0 && p.pc < (int)instrucciones.size()) {
            // Simular interrupción aleatoria (20% de probabilidad)
            // Solo si el quantum restante está entre 1 y 3 inclusive
            if (p.quantum >= 1 && p.quantum <= 3) {
                int prob = rand() % 100;
                if (prob < 30) {
                    strcpy(p.estado, "Bloqueado");
                    cout << "   [Interrupción] Proceso " << p.pid << " se bloquea antes de agotar el quantum." << endl;
                    if (log.is_open()) log << "[Interrupción] Proceso " << p.pid << " se bloquea antes de agotar el quantum en ciclo " << ciclosEjecutados+1 << "." << endl;
                    break; // Sale del ciclo, simulando bloqueo
                }
            }

            string inst = instrucciones[p.pc];
            // Eliminar espacios y comas extra
            for (auto& c : inst) if (c == ',') c = ' ';
            istringstream iss(inst);
            string op;
            iss >> op;

            // Si la instruccion es INC entonces incrementa el registro correspondiente
            if (op == "INC") {
                string reg;
                iss >> reg;
                if (reg == "AX") p.ax++;
                else if (reg == "BX") p.bx++;
                else if (reg == "CX") p.cx++;
            } 
            // Si la instruccion es ADD, SUB o MUL entonces realiza la operacion correspondiente
            else if (op == "ADD" || op == "SUB" || op == "MUL") {
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
                    else val = stoi(reg2);
                    if (op == "ADD") *dest += val;
                    else if (op == "SUB") *dest -= val;
                    else if (op == "MUL") *dest *= val;
                }
            } 
            else if (op == "NOP") {
                // No hacer nada
            } 
            else if (op == "JMP") {
                int salto;
                if (iss >> salto) {
                    p.pc = salto - 1;
                }
            }

            // Incrementa el contador de programa y el quantum
            p.pc++;
            ciclosEjecutados++;
            p.quantum--;

            // Muestra el estado del proceso después de cada instrucción la tabla
            cout << "   Ciclo " << ciclosEjecutados << " | PC = " << p.pc << " | Quantum restante = " << p.quantum << " | Inst: " << inst << endl;
            if (log.is_open()) log << "PID " << p.pid << " | Ciclo " << ciclosEjecutados << " | PC=" << p.pc << " | Quantum=" << p.quantum << " | Inst: " << inst << endl;
            mostrarProcesos(procesos);
            cout << "\n";
        }

        // Verifica si el proceso ha terminado todas sus instrucciones o si se ha agotado su quantum
        if (p.pc >= (int)instrucciones.size()) {
            cout << "Proceso " << p.pid << " ha terminado todas sus instrucciones." << endl;
            if (log.is_open()) log << "Proceso " << p.pid << " ha terminado todas sus instrucciones." << endl;
            strcpy(p.estado, "Terminado");
            mostrarProcesos(procesos);
        } else if (p.quantum == 0) {
            cout << "Proceso " << p.pid << " ha agotado su quantum." << endl;
            if (log.is_open()) log << "Proceso " << p.pid << " ha agotado su quantum." << endl;
            strcpy(p.estado, "Terminado");
            mostrarProcesos(procesos);
        } else if (strcmp(p.estado, "Bloqueado") == 0) {
            if (log.is_open()) log << "Proceso " << p.pid << " quedó bloqueado (interrumpido)." << endl;
        }
    }
    cout << "\n=== FIN DE LA PLANIFICACIÓN ===\n" << endl;
    if (log.is_open()) {
        log << "\n=== FIN DE LA PLANIFICACIÓN ===\n" << endl;
        log.close();
    }
}
