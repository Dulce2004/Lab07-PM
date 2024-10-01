/*
*------------------------------------------
* pthread_mutex - Ejercicio02
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
* Descripción: Programa que simula el proceso de producción de café en una planta donde se utiliza
* paralelización con hilos para controlar la operación de tostadoras y una empacadora. Utilizando pthread junto con 
* mutex_lock y mutex_unlock, el programa sincroniza las acciones de las tostadoras, que producen libras de café tostado, 
* y la empacadora, que empaqueta el café en bolsas.
*------------------------------------------
*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h> // Para la función sleep

// Variables globales
int silo = 0;        // Libras de café en el silo
int bodega = 0;      // Libras de café en la bodega
const int max_bolsas = 400; // Cantidad total de bolsas a producir

// Mutex y variables condicionales
pthread_mutex_t candado;
pthread_cond_t lleno, vacio;

// Función para la tostadora 1
void* tostadora1(void* arg) {
    for (int i = 0; i < max_bolsas / 2; ++i) {
        sleep(1); // Simula el tiempo de trabajo de la tostadora
        pthread_mutex_lock(&candado);
        
        // Esperar a que haya espacio en el silo si ya hay 5 libras
        while (silo >= 5) {
            pthread_cond_wait(&vacio, &candado);
        }
        
        // Agregar 1 libra de café al silo
        silo++;
        printf("Tostadora 1 produjo: 1 lb de cafe tostado\n");
        
        // Notificar a la empacadora si hay suficiente café
        if (silo == 5) {
            pthread_cond_signal(&lleno);
        }
        pthread_mutex_unlock(&candado);
    }
    return NULL;
}

// Función para la tostadora 2
void* tostadora2(void* arg) {
    for (int i = 0; i < max_bolsas / 2; ++i) {
        sleep(1); // Simula el tiempo de trabajo de la tostadora
        pthread_mutex_lock(&candado);
        
        // Esperar a que haya espacio en el silo si ya hay 5 libras
        while (silo >= 5) {
            pthread_cond_wait(&vacio, &candado);
        }
        
        // Agregar 1 libra de café al silo
        silo++;
        printf("Tostadora 2 produjo: 1 lb de cafe tostado\n");
        
        // Notificar a la empacadora si hay suficiente café
        if (silo == 5) {
            pthread_cond_signal(&lleno);
        }
        pthread_mutex_unlock(&candado);
    }
    return NULL;
}

// Función para la empacadora
void* empacadora(void* arg) {
    while (bodega < max_bolsas) {
        pthread_mutex_lock(&candado);
        
        // Esperar hasta que haya exactamente 5 libras de café en el silo
        while (silo < 5) {
            pthread_cond_wait(&lleno, &candado);
        }

        // Empacar 5 libras de café, asegurando que el proceso ocurra correctamente
        for (int i = 0; i < 5 && bodega < max_bolsas; ++i) {
            sleep(1); // Simula el tiempo de empaquetado
            silo--;   // Disminuir el silo después de procesar
            bodega++; // Aumentar la bodega después de empacar
            printf("Lbs de cafe en silo: %d\n", silo);
            printf("Empacadora produjo: 1 bolsa de 1 lb de cafe\n");
            printf("Lbs de cafe en bodega: %d\n", bodega);
        }
        
        // Notificar a las tostadoras que pueden seguir produciendo
        pthread_cond_broadcast(&vacio);
        pthread_mutex_unlock(&candado);
    }
    return NULL;
}

int main() {
    // Inicializar mutex y variables condicionales
    pthread_mutex_init(&candado, NULL);
    pthread_cond_init(&lleno, NULL);
    pthread_cond_init(&vacio, NULL);
    
    // Crear los hilos
    pthread_t hilo_tostadora1, hilo_tostadora2, hilo_empacadora;
    pthread_create(&hilo_tostadora1, NULL, tostadora1, NULL);
    pthread_create(&hilo_tostadora2, NULL, tostadora2, NULL);
    pthread_create(&hilo_empacadora, NULL, empacadora, NULL);
    
    // Esperar a que los hilos terminen
    pthread_join(hilo_tostadora1, NULL);
    pthread_join(hilo_tostadora2, NULL);
    pthread_join(hilo_empacadora, NULL);
    
    // Destruir mutex y variables condicionales
    pthread_mutex_destroy(&candado);
    pthread_cond_destroy(&lleno);
    pthread_cond_destroy(&vacio);
    
    printf("Producción completada. Lbs de cafe en bodega: %d\n", bodega);
    return 0;
}