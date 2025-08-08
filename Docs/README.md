
# Project 1 - Procplanner

## Tabla de Contenidos

1. [Descripción](#descripción)
2. [Requisitos](#requisitos)
3. [Integrantes](#integrantes)
4. [Instrucciones de ejecución](#instrucciones-de-ejecución)

## Descripción

Este proyecto simula el cambio de contexto entre procesos, representando cómo un sistema operativo gestiona el tiempo de CPU entre procesos activos. Escrito en C++ para ejecutarse desde consola. Utiliza una estructura clave-valor (`unordered_map`) para la gestión eficiente de procesos por PID.

Además, implementa un simulador de planificación de procesos basado en el algoritmo round-robin, orientado a la enseñanza y experimentación en sistemas operativos. El objetivo es mostrar de forma visual y comprensible cómo un sistema operativo reparte el tiempo de CPU entre varios procesos, gestionando el cambio de contexto y el avance de cada proceso según un quantum definido.

Características principales:

- **Simulación realista de procesos:** Cada proceso cuenta con registros (`AX`, `BX`, `CX`), un contador de programa (PC), estado (`Listo`, `Ejecutando`, `Terminado`) y una lista de instrucciones que se ejecutan paso a paso.
- **Gestión eficiente con clave-valor:** Se utiliza un `unordered_map` para almacenar y acceder rápidamente a los procesos por su PID, facilitando operaciones como búsqueda y actualización.
- **Entrada flexible:** Los procesos y sus instrucciones se definen en archivos de texto editables, permitiendo modificar fácilmente los escenarios de prueba.
- **Planificador round-robin:** El núcleo del simulador es un planificador que asigna la CPU a cada proceso en turnos iguales (quantum), realizando cambios de contexto automáticos y mostrando el estado de todos los procesos en cada ciclo.
- **Visualización clara:** En cada ciclo, se imprime una tabla que muestra el estado actual de todos los procesos, los valores de sus registros y el quantum restante, permitiendo analizar el comportamiento del sistema en tiempo real o exportar la salida para análisis posterior.

## Requisitos

- Linux (o WSL)
- Compilador `g++`
- Consola/Bash terminal

## Integrantes

- Juan Esteban Trujillo
- Camilo Alvarez
- Samuel Calderon

## Instrucciones de ejecución

Sigue estos pasos para obtener, compilar y ejecutar el simulador:

1. Clona el repositorio:
   ```bash
   git clone https://github.com/JuanTrujilloM/Project1-Procplanner.git
   cd Project1-Procplanner
   ```
2. Compila el código fuente con:
   ```bash
   g++ src/procplanner.cpp src/proceso.cpp -o bin/procplanner
   ```
3. Ejecuta el simulador con el archivo de entrada de procesos:
   ```bash
   ./bin/procplanner input/procesos.txt
   ```
4. Modifica los archivos en `input/` para experimentar con diferentes procesos e instrucciones.

### Ejemplo de salida

Al ejecutar el simulador, verás una tabla similar a la siguiente en la consola:

```
PID	PC	AX	BX	CX	Quantum	Estado
---	--	--	--	--	-------	------
1	1	3	3	2	7	Ejecutando
2	0	5	2	4	5	Listo
3	0	8	1	6	6	Listo
4	0	3	7	2	4	Listo
5	0	0	0	0	3	Listo
```

Cada ciclo muestra el avance de los procesos, cambios de contexto y finalización.

