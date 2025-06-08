#include "sensoristica.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "sensoristica.h"
#include <stdio.h>
#include <stdlib.h>
#include "terna.h"
#define MIN_VALUE 724
#define MAX_VALUE 1000



void selectionSort(SerraDati serre[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (serre[j].umidita_terreno < serre[min_idx].umidita_terreno) {
                min_idx = j;
            }
        }
        // Scambio
        SerraDati temp = serre[i];
        serre[i] = serre[min_idx];
        serre[min_idx] = temp;
    }
}

void selectiontemp(SerraDati serre[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (serre[j].temperatura < serre[min_idx].temperatura) {
                min_idx = j;
            }
        }
        // Scambio
        SerraDati temp = serre[i];
        serre[i] = serre[min_idx];
        serre[min_idx] = temp;
    }
}

