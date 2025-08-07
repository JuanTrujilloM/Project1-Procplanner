# procplanner

Simulación de planificación de procesos estilo round-robin en entornos Linux.

Este proyecto simula el cambio de contexto entre procesos, representando cómo un sistema operativo gestiona el tiempo de CPU entre procesos activos. Escrito en C++ para ejecutarse desde consola.

## Descripción

`procplanner` permite:

- Simular procesos con registros (`AX`, `BX`, `CX`) y estado (`Listo`, `Ejecutando`, `Terminado`)
- Cargar procesos desde un archivo `.txt`
- Ejecutar instrucciones simuladas paso a paso (`ADD`, `INC`, `NOP`, `SUB`)
- Implementar un planificador round-robin con cambio de contexto
- Visualizar claramente qué ocurre en cada ciclo de CPU

## Requisitos

- Linux (WSL)
- Compilador `gcc` o `g++`
- Consola/Bash terminal

## Compilación

```bash
#Para C++
g++ -o procplanner main.cpp
```

## Ejecución 

```bash
./procplanner
./procplanner -f procesos.txt
```

