# Redis Stress Test

Este programa permite realizar pruebas de estrés a un servidor de Redis mediante la inserción de registros utilizando conexiones concurrentes con la librería Pthreads o con OpenMP.

## Pre-requisitos

* Un servidor de Redis.
* El compilador `gcc`
* La biblioteca de funciones [`hiredis`](https://github.com/redis/hiredis)

## Instrucciones de uso

1. Descargue el repositorio a una carpeta de su computadora utilizando el comando `git clone`.
2. Cámbiese a la carpeta del proyecto.
3. Compile el código con el comando:

`gcc example-redis-c.c -o test-redis-c  -lhiredis -I /usr/local/include/hiredis/ -pthread`
 
 o en caso de OpenMP:
 
 `gcc main.c -o test-redis-c  -lhiredis -I /usr/local/include/hiredis/ -fopenmp`
 
4. Ejecute el programa con el comando:

`./test-redis-c [HOSTNAME | IP] [PORT]`

