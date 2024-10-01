/*
*------------------------------------------
* pthread_mutex - Ejercicio1 
* -----------------------------------------
* UNIVERSIDAD DEL VALLE DE GUATEMALA
* FACULTAD DE INGENIERÍA
* DEPARTAMENTO DE CIENCIA DE LA COMPUTACIÓN
* CC3086 - Programacion de Microprocesadores
*
*------------------------------------------
* Autor 1: Dulce Ambrosio - 231143
* Autor 2: Javier Linares - 231135
*------------------------------------------
* Descripción: Se realizó un programa donde
* se realizaron diferentes cálculos con datos
* dados, poniendo en uso el mutex_lock y el
* mutex_unlock, se imprimen los datos de los
* productos y los calculos realizados.
*------------------------------------------
*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h> 
#include <string.h> 
#include <semaphore.h> 

// Declaración de un mutex 
pthread_mutex_t mutex;

// Se crearon los arreglos con los datos
char *producto[8] = {
    "Porcion pastel de chocolate", "White mocha", 
    "Cafe americano 8onz", "Latte 8onz", 
    "Toffe coffee", "Cappuccino 8onz", 
    "S´mores Latte", "Cafe tostado molido"
};
int unidadesJ[8] = {300, 400, 1590, 200, 390, 1455, 800, 60}; // Unidades vendidas en Julio
float unitario[8] = {60, 32, 22, 24, 28, 24, 32, 60};         // Precio unitario por producto
float costo[8] = {20, 19.20, 13.20, 17.20, 20.10, 17.20, 23, 20}; // Costo de cada producto
int unidadesA[8] = {250, 380, 800, 250, 600, 1200, 1540, 15}; // Unidades vendidas en Agosto

// Arreglos para almacenar las ventas y utilidades por mes
float ventasA[8];
float ventasJ[8];
float utilidadA[8];
float utilidadJ[8];

// Variables para acumular ventas y utilidades totales por mes
float tventasJ = 0;
float tventasA = 0;
float tutilidadesJ = 0;
float tutilidadesA = 0;

// Función ejecutada por cada hilo para calcular las ventas y utilidades de Julio
void* Julio(void* arg) {
    int id = *(int*)arg; // Identificador del producto asignado a este hilo

    // Bloquear el mutex antes de imprimir
    pthread_mutex_lock(&mutex);
    printf("Calculo hilo %d iniciado \n", id); // Indicar el inicio del cálculo
    pthread_mutex_unlock(&mutex);

    // Calcular ventas y utilidad para el producto de Julio
    ventasJ[id] = unidadesJ[id] * unitario[id];
    utilidadJ[id] = ventasJ[id] - (unidadesJ[id] * costo[id]);

    // Bloquear el mutex para actualizar las ventas y utilidades totales
    pthread_mutex_lock(&mutex);
    tventasJ += ventasJ[id]; // Acumular ventas totales de Julio
    tutilidadesJ += utilidadJ[id]; // Acumular utilidades totales de Julio
    printf("Calculo hilo %d terminado \n", id); // Indicar que el cálculo ha terminado
    pthread_mutex_unlock(&mutex);

    sleep(1); // Pausar el hilo por 1 segundo para simular un retraso

    return NULL;
}

// Función ejecutada por cada hilo para calcular las ventas y utilidades de Agosto
void* Agosto(void* arg) {
    int id = *(int*)arg; // Identificador del producto asignado a este hilo

    // Bloquear el mutex antes de imprimir
    pthread_mutex_lock(&mutex);
    printf("Calculo hilo %d iniciado \n", id); // Indicar el inicio del cálculo
    pthread_mutex_unlock(&mutex);

    // Calcular ventas y utilidad para el producto de Agosto
    ventasA[id] = unidadesA[id] * unitario[id];
    utilidadA[id] = ventasA[id] - (unidadesA[id] * costo[id]);

    // Bloquear el mutex para actualizar las ventas y utilidades totales
    pthread_mutex_lock(&mutex);
    tventasA += ventasA[id]; // Acumular ventas totales de Agosto
    tutilidadesA += utilidadA[id]; // Acumular utilidades totales de Agosto
    printf("Calculo hilo %d terminado \n", id); // Indicar que el cálculo ha terminado
    pthread_mutex_unlock(&mutex);

    sleep(1); // Pausar el hilo por 1 segundo para simular un retraso

    return NULL;
}

int main() {
    pthread_t threads[8]; // Arreglo de hilos para ejecutar cálculos
    int thread_ids[8]; // Identificadores de los hilos

    // Inicializar el mutex antes de usarlo
    pthread_mutex_init(&mutex, NULL);

    // Crear hilos para realizar los cálculos de Julio
    for (int i = 0; i < 8; i++) {
        thread_ids[i] = i; // Asignar el identificador al hilo
        pthread_create(&threads[i], NULL, Julio, (void*)&thread_ids[i]); // Crear hilo para Julio
    }

    // Esperar a que todos los hilos de Julio terminen su ejecución
    for (int i = 0; i < 8; i++) {
        pthread_join(threads[i], NULL); // Unir los hilos
    }

    // Imprimir reporte de Julio
    printf("\n-------------------------------\n");
    printf("\nREPORTE DEL MES DE JULIO\n");
    printf("\n--- Monto Ventas por Producto ---\n");
    for (int i = 0; i < 8; i++) {
        printf("- %s: Q%.2f\n", producto[i], ventasJ[i]); // Ventas por producto en Julio
    }
    
    printf("\n--- Utilidad por Producto ---\n");
    for (int i = 0; i < 8; i++) {
        printf("- %s: Q%.2f\n", producto[i], utilidadJ[i]); // Utilidad por producto en Julio
    }

    printf("\n--- Monto Total Ventas del Mes: Q%.2f\n", tventasJ); // Total de ventas en Julio
    printf("\n--- Utilidad del mes: Q%.2f\n", tutilidadesJ); // Total de utilidades en Julio

    // Destruir el mutex tras completar los cálculos de Julio
    pthread_mutex_destroy(&mutex);

    // -------------------------------------------------------------------------------

    // Inicializar nuevamente el mutex para el cálculo de Agosto
    printf("\n----------------------------------------------------------------\n");
    pthread_mutex_init(&mutex, NULL);

    // Crear hilos para realizar los cálculos de Agosto
    for (int i = 0; i < 8; i++) {
        thread_ids[i] = i; // Asignar el identificador al hilo
        pthread_create(&threads[i], NULL, Agosto, (void*)&thread_ids[i]); // Crear hilo para Agosto
    }

    // Esperar a que todos los hilos de Agosto terminen su ejecución
    for (int i = 0; i < 8; i++) {
        pthread_join(threads[i], NULL); // Unir los hilos
    }

    // Imprimir reporte de Agosto
    printf("\n-------------------------------\n");
    printf("\nREPORTE DEL MES DE AGOSTO\n");
    printf("\n--- Monto Ventas por Producto ---\n");
    for (int i = 0; i < 8; i++) {
        printf("- %s: Q%.2f\n", producto[i], ventasA[i]); // Ventas por producto en Agosto
    }

    printf("\n--- Utilidad por Producto ---\n");
    for (int i = 0; i < 8; i++) {
        printf("- %s: Q%.2f\n", producto[i], utilidadA[i]); // Utilidad por producto en Agosto
    }

    printf("\n--- Monto Total Ventas del Mes: Q%.2f\n", tventasA); // Total de ventas en Agosto
    printf("\n--- Utilidad del mes: Q%.2f\n", tutilidadesA); // Total de utilidades en Agosto

    pthread_mutex_destroy(&mutex);

    return 0;
}
