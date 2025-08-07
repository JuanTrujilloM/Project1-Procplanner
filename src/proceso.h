#ifndef PROCESO_H
#define PROCESO_H

#include <unordered_map>
#include <string>

// Estructura general de un proceso
struct Proceso {
    int pid;    // Identificador del proceso
    int pc;     // Contador de programa
    int ax, bx, cx;     // Registros del proceso
    int quantum;    // Quantum de tiempo asignado al proceso
    char estado[10];    // Estado del proceso (Listo, Ejecutando, Terminado)
};

// Funcion para cargar procesos desde un archivo
std::unordered_map<int, Proceso> cargarDesdeArchivo(const std::string& nombreArchivo);

// Muestra los procesos en pantalla
void mostrarProcesos(const std::unordered_map<int, Proceso>& procesos);

// Ejecuta los procesos en un planificador Round Robin
void ejecutarProcesos(std::unordered_map<int, Proceso>& procesos);

#endif // PROCESO_H
