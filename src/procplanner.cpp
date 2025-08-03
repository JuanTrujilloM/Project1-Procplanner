#include <iostream>  
#include <fstream>   
#include <vector>    
#include <sstream>   
#include <string>    
#include <cstring>   

using namespace std; 

// Esta estructura define la información que almacena cada proceso
typedef struct {
    int pid;         
    int pc;          
    int ax, bx, cx;  
    int quantum;     
    char estado[10]; 
} Proceso;

// Esta función lee procesos desde un archivo de texto y los carga en memoria
vector<Proceso> cargarDesdeArchivo(const string& nombreArchivo) {
    vector<Proceso> procesos; 
    ifstream archivo(nombreArchivo); 
    
    // Verifica si el archivo se abrió correctamente
    if (!archivo.is_open()) {
        cerr << "Error: No se pudo abrir el archivo " << nombreArchivo << endl;
        return procesos; 
    }
    
    string linea; 
    // Lee el archivo línea por línea
    while (getline(archivo, linea)) {
        // Busca líneas que contengan "PID:" (formato esperado)
        if (linea.find("PID:") != string::npos) {
            Proceso p; // Crea un nuevo proceso
            
            // Inicializa valores por defecto
            p.pc = 0; 
            strcpy(p.estado, "Listo"); 
            
            istringstream ss(linea);
            string temp; 
            
            ss >> temp >> p.pid; 
            
            // Procesa el resto de la línea palabra por palabra
            while (ss >> temp) {
                if (temp.find("AX=") == 0) {
                    p.ax = stoi(temp.substr(3)); // Extrae número después de "AX="
                } else if (temp.find("BX=") == 0) {
                    p.bx = stoi(temp.substr(3)); // Extrae número después de "BX="
                } else if (temp.find("CX=") == 0) {
                    p.cx = stoi(temp.substr(3)); // Extrae número después de "CX="
                } else if (temp.find("Quantum=") == 0) {
                    p.quantum = stoi(temp.substr(8)); // Extrae número después de "Quantum="
                } else if (temp.find("Q=") == 0) {
                    p.quantum = stoi(temp.substr(2)); 
                }
            }
            
            procesos.push_back(p); // Agrega el proceso al vector
        }
    }
    
    archivo.close(); 
    return procesos; 
}

// Muestra los procesos cargados en formato de tabla organizada
void mostrarProcesos(const vector<Proceso>& procesos) {
    cout << "\n=== PROCESOS CARGADOS ===" << endl;
    
    // Imprime encabezados de la tabla
    cout << "PID\tPC\tAX\tBX\tCX\tQuantum\tEstado" << endl;
    cout << "---\t--\t--\t--\t--\t-------\t------" << endl;
    
    // Recorre todos los procesos e imprime sus datos
    for (const auto& p : procesos) {
        cout << p.pid << "\t" << p.pc << "\t" << p.ax << "\t" 
             << p.bx << "\t" << p.cx << "\t" << p.quantum << "\t" 
             << p.estado << endl;
    }
}

void ejecutarProcesos(vector<Proceso>& procesos) {
    cout << "\n=== INICIO DE PLANIFICACIÓN ROUND ROBIN ===\n" << endl;

    for (size_t i = 0; i < procesos.size(); ++i) {
        Proceso& p = procesos[i];

        // Si no es el primer proceso, imprimir cambio de contexto
        if (i > 0) {
            Proceso& anterior = procesos[i - 1];
            cout << "[Cambio de contexto]" << endl;
            cout << "Guardando estado de Proceso " << anterior.pid
                 << ": PC=" << anterior.pc
                 << ", AX=" << anterior.ax
                 << ", BX=" << anterior.bx << endl;
            cout << "Cargando estado de Proceso " << p.pid
                 << ": PC=" << p.pc
                 << ", AX=" << p.ax
                 << ", BX=" << p.bx << endl << endl;
        }

        strcpy(p.estado, "Ejecutando");   // Cambia estado

        cout << ">> Ejecutando PID " << p.pid << " durante " << p.quantum << " ciclos." << endl;

        cout << "[Estado] Proceso " << p.pid << " = " << p.estado << endl;

        for (int i = 0; i < p.quantum; ++i) {
            p.pc++;  // Avanza el contador de programa
            cout << "   Ciclo " << i + 1 << " | PC = " << p.pc << endl;
        }

        strcpy(p.estado, "Terminado");  // Finaliza
    }

    cout << "\n=== FIN DE LA PLANIFICACIÓN ===\n" << endl;
}

int main(int argc, char* argv[]) {
    // Verifica que se proporcione exactamente un argumento (nombre del archivo)
    if (argc != 2) {
        cout << "Uso: ./procplanner archivo.txt" << endl;
        cout << "Ejemplo: ./procplanner procesos.txt" << endl;
        return 1; // Código de error
    }
    
    // Carga los procesos desde el archivo especificado
    vector<Proceso> procesos = cargarDesdeArchivo(argv[1]);
    
    // Verifica si se cargaron procesos exitosamente
    if (procesos.empty()) {
        cout << "No se cargaron procesos del archivo." << endl;
        return 1; 
    }
    
    // Muestra los procesos cargados en pantalla
    mostrarProcesos(procesos);
    ejecutarProcesos(procesos);
    mostrarProcesos(procesos);
    
    return 0; 
}