// Inclusión del encabezado con la estructura y las declaraciones
#include "proceso.h"
#include <iostream>
using namespace std;

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
    cout << "Procesos cargados desde el archivo:" << endl;
    mostrarProcesos(procesos);
    ejecutarProcesos(procesos);
    mostrarProcesos(procesos);
    
    return 0; 
}