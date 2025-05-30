#ifndef TERNA_H
#define TERNA_H#
#include "sensoristica.h"
// Struttura per i valori fissi delle serre
typedef struct {
    int temperatura_fissa;    // Temperatura fissa per la serra
    int umidita_terreno_fissa; // Umidità fissa del terreno
    int umidita_aria;
} ValoriFissi;

// Array dei valori fissi per ciascuna serra
extern ValoriFissi valori_fissi[];

// Funzione di controllo della temperatura
void controlloTemperatura(SerraDati serre[], int n);

#endif // TERNA_H
