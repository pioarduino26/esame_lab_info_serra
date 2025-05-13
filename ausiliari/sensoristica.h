#ifndef SENSORISTICA_H
#define SENSORISTICA_H
// uso la libreria "time.h"
#include <time.h>
#include <stdio.h>
// Definizione della struttura Pianta
typedef struct {
    char nome[20]; // dichiaro un array di caratteri di 20 elementi
    int umidita_min;
    int umidita_max;
    int resistenza_insetticida; // 1 = Resistente, 0 = Debole
} Pianta;

// Definizione della struttura che contiene i dati della serra (temperatura, umidità....)
typedef struct {
    int temperatura;
    int umidita;
    int luce;
    int umidita_terreno;
    int livello_acqua;
    struct tm orario;
    Pianta pianta;
} SerraDati;
void leggiSensori(SerraDati *dati);
void controllaIrrigazione(int umidita_terreno, Pianta pianta, struct tm orario);
void selectionSort(SerraDati serre[], int n);
void irrigazioneInsetticida(SerraDati serre[], int *n);
void strappaPiantaESostituisci(SerraDati *serra);
void salvaStoricoRec(SerraDati serre[], int n, int index, FILE* fp);
const char* determinaStagione(int mese); // Restituisce la stagione in base al mese
