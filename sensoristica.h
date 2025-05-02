#ifndef SENSORISTICA_H
#define SENSORISTICA_H

#include <time.h>

// Definizione della struttura Pianta
typedef struct {
    char nome[20];
    int umidita_min;
    int umidita_max;
    int resistenza_insetticida; // 1 = Resistente, 0 = Debole
} Pianta;

// Definizione della struttura che contiene i dati della serra
typedef struct {
    int temperatura;
    int umidita;
    int luce;
    int umidita_terreno;
    int livello_acqua;
    struct tm orario;
    Pianta pianta;
} SerraDati;
