#ifndef PROCESO_H
#define PROCESO_H

#include <vector>
#include <string>

struct Proceso {
    int pid;
    int pc;
    int ax, bx, cx;
    int quantum;
    char estado[10];
};

std::vector<Proceso> cargarDesdeArchivo(const std::string& nombreArchivo);
void mostrarProcesos(const std::vector<Proceso>& procesos);
void ejecutarProcesos(std::vector<Proceso>& procesos);

#endif // PROCESO_H
