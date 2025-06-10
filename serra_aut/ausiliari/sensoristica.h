#ifndef SENSORISTICA_H
#define SENSORISTICA_H
#include <time.h>
#include <stdio.h>


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
    int ventola_riciclo;
    int ventola_raffreddamento;
    int led;
    int buzzer;               // Stato buzzer (0=spento, 1= acceso)
    int tetto;
    int motore_acqua;
} SerraDati;

// Prototipi delle funzioni
void leggiSensori(SerraDati *dati, int index);
const char* determinaStagione(int mese); // Restituisce la stagione in base al mese
void selectionSort(SerraDati serre[], int n);
void irrigazioneInsetticida(SerraDati serre[], int *n);
void strappaPiantaESostituisci(SerraDati *serra);
void salvaStoricoRec(SerraDati serre[], int n, int index, FILE* fp);
#endif // SENSORISTICA_H

