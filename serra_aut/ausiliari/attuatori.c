#include "attuatori.h"
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// Variabile globale per lo stato del tetto
int tetto_aperto = 0; // 0 = chiuso, 1 = aperto
time_t ultima_pioggia = 0;

// Inizializza il generatore di numeri casuali
void inizializzaGeneratoreCasuale() {
    srand(time(NULL));
}

// Implementazione di rilevaPioggia
int rilevaPioggia() {
    time_t t = time(NULL);
    struct tm *orario = localtime(&t);
    const char* stagione = determinaStagione(orario->tm_mon + 1);

    int probabilita_pioggia;

    if (strcmp(stagione, "Estate") == 0) {
        probabilita_pioggia = 8;  // Solo 8% di probabilità di pioggia
    } else if (strcmp(stagione, "Inverno") == 0) {
        probabilita_pioggia = 70;
    } else {
        probabilita_pioggia = 40; // Primavera e Autunno
    }

    int random = rand() % 100;
    return random < probabilita_pioggia; // 1 = pioggia, 0 = no
}
// Implementazione aggiornata di controllaTetto
void controllaTetto(SerraDati *serra) {
    int pioggia = rilevaPioggia();
    const char* stagione = determinaStagione(serra->orario.tm_mon + 1);

    if (pioggia) {
        tetto_aperto = 0;
    } else {
        tetto_aperto = 1;
    }

    serra->tetto = tetto_aperto;
}

// Implementazione aggiornata di controllaVentolaRaffreddamento
void controllaVentolaRaffreddamento(int temperatura, struct tm orario,SerraDati *serra) {
    int velocita;
    if (orario.tm_hour >= 6 && orario.tm_hour <= 18) {
        velocita = (temperatura - 15) * 12;
    } else {
        velocita = (temperatura - 15) * 6;
    }

    // Regola la velocità in base allo stato del tetto
    if (tetto_aperto) {
        velocita = velocita * 0.7; // Riduci la velocità del 30% se il tetto è aperto
    }

    if (velocita > 255) velocita = 255;
    if (velocita < 50) velocita = 20;

    printf("Ventola raffreddamento: %d (Orario: %02d:%02d)\n", velocita, orario.tm_hour, orario.tm_min);
    serra->ventola_raffreddamento = velocita;
}

// Implementazione aggiornata di controllaVentolaRiciclo
// Implementazione aggiornata di controllaVentolaRiciclo
void controllaVentolaRiciclo(struct tm orario, SerraDati *serra) {
    int velocita = (orario.tm_hour - 1) * 10 + 40;
    if (velocita < 40) velocita = 40;
    if (velocita > 255) velocita = 255;

    // Aumenta la velocità in base allo stato del tetto
    if (!tetto_aperto) {
        velocita = velocita * 1.2; // Aumenta la velocità del 20% se il tetto è chiuso
        if (velocita > 255) velocita = 255; // Limita la velocità massima
    }

    printf("Ventola riciclo aria: %d\n", velocita);

    // Aggiorna il campo ventola_riciclo nella struttura SerraDati
    serra->ventola_riciclo = velocita;
}

// Implementazione aggiornata di controllaIrrigazione
void controllaIrrigazione(int umidita_terreno, Pianta pianta, struct tm orario) {
    const char* stagione = determinaStagione(orario.tm_mon + 1);
    int soglia = pianta.umidita_min;

    if (strcmp(stagione, "Estate") == 0) {
        soglia += 50; // In estate, il suolo perde umidità più rapidamente
    } else if (strcmp(stagione, "Inverno") == 0) {
        soglia -= 30; // In inverno, il suolo trattiene più umidità
    }

    // Monitora l'umidità in relazione allo stato del tetto
    if (tetto_aperto && umidita_terreno < soglia) {
        printf("[%s] Motore acqua: ACCESO (Stagione: %s, Tetto: APERTO)\n", pianta.nome, stagione);
    } else if (umidita_terreno >= soglia && umidita_terreno <= pianta.umidita_max) {
        printf("[%s] Motore acqua: SPENTO\n", pianta.nome);
    } else {
        printf("[%s] Motore acqua: ACCESO (Stagione: %s)\n", pianta.nome, stagione);
    }
}
// effettuo il controllo sulla presenza o meno di luce
void controllaIlluminazione(int luce, struct tm orario) {
    if (orario.tm_hour >= 20 || orario.tm_hour < 6) {
        // Se e' notte, accendi la luce se necessario
        if (luce < 300) {
            printf("Accendi la luce artificiale (Notte)\n");
        } else {
            printf("Luce naturale sufficiente (Notte)\n");
        }
    } else {
        printf("Spegni la luce artificiale (Giorno)\n");
    }
}
// controllo livello dell'acqua e azioni sugli attuatori
void controllaLivelloAcqua(int livello_acqua) {
    if (livello_acqua < 200) {
        printf("Livello acqua basso! LED ROSSO ON, Buzzer ON\n");
    } else {
        printf("Livello acqua sufficiente. LED VERDE ON\n");
    }
}
