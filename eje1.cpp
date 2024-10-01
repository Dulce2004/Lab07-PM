#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h> 
#include <string.h> 
#include <semaphore.h> 

pthread_mutex_t mutex;

char *producto[8] = {"Porcion pastel de chocolate", "White mocha", "Cafe americano 8onz", "Latte 8onz", "Toffe coffee", "Cappuccino 8onz", "S´mores Latte", "Cafe tostado molido"};
int unidadesJ[8] = {300, 400, 1590, 200, 390, 1455, 800, 60};
float unitario[8] = {60, 32, 22, 24, 28, 24, 32, 60}; 
float costo[8] = {20, 19.20, 13.20, 17.20, 20.10, 17.20, 23, 20};
int unidadesA[8] = {250, 380, 800, 250, 600, 1200, 1540, 15};

float ventasA[8];
float ventasJ[8];
float utilidadA[8];
float utilidadJ[8];
float tventasJ = 0;
float tventasA = 0;
float tutilidadesJ = 0;
float tutilidadesA = 0;

void* Julio(void* arg) {
    int id = *(int*)arg;

    pthread_mutex_lock(&mutex);
    printf("Calculo hilo %d iniciado \n", id);
    pthread_mutex_unlock(&mutex);

    ventasJ[id] = unidadesJ[id] * unitario[id];
    utilidadJ[id] = ventasJ[id] - (unidadesJ[id] * costo[id]);

    pthread_mutex_lock(&mutex);
    tventasJ += ventasJ[id];
    tutilidadesJ += utilidadJ[id];
    printf("Calculo hilo %d terminado \n", id);
    pthread_mutex_unlock(&mutex);

    sleep(1); 

    return NULL;
}

void* Agosto(void* arg) {
    int id = *(int*)arg;

    pthread_mutex_lock(&mutex);
    printf("Calculo hilo %d iniciado \n", id);
    pthread_mutex_unlock(&mutex);

    ventasA[id] = unidadesA[id] * unitario[id];
    utilidadA[id] = ventasA[id] - (unidadesA[id] * costo[id]);

    pthread_mutex_lock(&mutex);
    tventasA += ventasA[id];
    tutilidadesA += utilidadA[id];
    printf("Calculo hilo %d terminado \n", id);
    pthread_mutex_unlock(&mutex);

    sleep(1); 

    return NULL;
}

int main() {
    pthread_t threads[8];
    int thread_ids[8];

    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < 8; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, Julio, (void*)&thread_ids[i]);
    }

    for (int i = 0; i < 8; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\n-------------------------------\n");
    printf("\nREPORTE DEL MES DE JULIO\n");
    printf("\n--- Monto Ventas por Producto ---\n");
    for (int i = 0; i < 8; i++) {
        printf("- %s: Q%.2f\n", producto[i], ventasJ[i]); 
    }
    

    printf("\n--- Utilidad por Producto ---\n");
    for (int i = 0; i < 8; i++) {
        printf("- %s: Q%.2f\n", producto[i], utilidadJ[i]); 
    }

    printf("\n--- Monto Total Ventas del Mes: Q%.2f\n", tventasJ);
    printf("\n--- Utilidad del mes: Q%.2f\n", tutilidadesJ);

    pthread_mutex_destroy(&mutex);

    //-----------------------------------------------------------------------------------

    printf("\n----------------------------------------------------------------\n");
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < 8; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, Agosto, (void*)&thread_ids[i]);
    }

    for (int i = 0; i < 8; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\n-------------------------------\n");
    printf("\nREPORTE DEL MES DE AGOSTO\n");
    printf("\n--- Monto Ventas por Producto ---\n");
    for (int i = 0; i < 8; i++) {
        printf("- %s: Q%.2f\n", producto[i], ventasA[i]);
    }

    printf("\n--- Utilidad por Producto ---\n");
    for (int i = 0; i < 8; i++) {
        printf("- %s: Q%.2f\n", producto[i], utilidadA[i]); 
    }

    printf("\n--- Monto Total Ventas del Mes: Q%.2f\n", tventasA);
    printf("\n--- Utilidad del mes: Q%.2f\n", tutilidadesA);

    pthread_mutex_destroy(&mutex);

    return 0;
}
