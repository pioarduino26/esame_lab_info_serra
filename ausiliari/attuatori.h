#ifndef ATTUATORI_H
#define ATTUATORI_H

#include "sensoristica.h"
#include <time.h>
#include <stdio.h>

// Variabile globale per lo stato del tetto
extern int tetto_aperto; // 0 = chiuso, 1 = aperto
extern time_t ultima_pioggia;

// Funzione per controllare il tetto della serra
void controllaTetto();

// Funzione per rilevare la pioggia
int rilevaPioggia();

// Altre funzioni di controllo degli attuatori...
void controllaIrrigazione(int umidita_terreno, Pianta pianta, struct tm orario);
void controllaLivelloAcqua(int livello_acqua);
void controllaVentolaRaffreddamento(int temperatura, struct tm orario);
void controllaVentolaRiciclo(struct tm orario);
void controllaIlluminazione(int luce, struct tm orario);

#endif // ATTUATORI_H
