#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "ausiliari/sensoristica.h"
#include <string.h>
// Funzione per stampare lo stato delle serre in base alla tipologia di pianta
void stampaSerre(SerraDati serre[], int n) {
    printf("\nStato delle serre:\n");
    for (int i = 0; i < n; i++) {
        printf("Serra %d: %s - Umidita' terreno: %d\n", i + 1, serre[i].pianta.nome, serre[i].umidita_terreno);
    }
    printf("\n");
}
// Stampa informazioni sulla serra: indice, nome della pianta e umidità del terreno
void salvaLog(SerraDati serre[], int n) {
    printf("Salva il log delle attivita' delle serre.\n");
    printf("Inserisci il nome del file in cui vuoi salvare il log: ");
    char nomeFile[100];
    scanf("%99s", nomeFile);

    FILE* fileSalvataggio = fopen(nomeFile, "w");// Apertura del file in scrittura
    if (!fileSalvataggio) {
        // Messaggio di errore se il file non può essere creato/aperto
        printf("Errore: Impossibile creare il file di salvataggio.\n");
        return;
    }
// Genera e salva i dati delle serre nel file specificato
    // Definizione della struttura per la lista concatenata
    typedef struct LogNode {
        char logData[256];
        struct LogNode* next;
    } LogNode;
// Creazione della lista concatenata temporanea
    LogNode* head = NULL;
    LogNode* tail = NULL;
    for (int i = 0; i < n; i++) {
        leggiSensori(&serre[i]);  // Aggiorna i dati della serra

        fprintf(fileSalvataggio, "Serra %d: %s\n", i + 1, serre[i].pianta.nome);
        fprintf(fileSalvataggio, "  Temperatura: %d°C\n", serre[i].temperatura);
        fprintf(fileSalvataggio, "  Umidita': %d%%\n", serre[i].umidita);
        fprintf(fileSalvataggio, "  Luce: %d\n", serre[i].luce);
        fprintf(fileSalvataggio, "  Umidita' terreno: %d\n", serre[i].umidita_terreno);
        fprintf(fileSalvataggio, "  Livello acqua: %d\n", serre[i].livello_acqua);
        fprintf(fileSalvataggio, "  Orario: %02d:%02d\n", serre[i].orario.tm_hour, serre[i].orario.tm_min);
        fprintf(fileSalvataggio, "  Stagione: %s\n", determinaStagione(serre[i].orario.tm_mon + 1));
        fprintf(fileSalvataggio, "-----------------------------\n");
        LogNode* newNode = (LogNode*)malloc(sizeof(LogNode));
        if (newNode == NULL) {
            printf("Errore: Memoria insufficiente per creare il nodo.\n");
            break;
        }
<<<<<<< Updated upstream
<<<<<<< Updated upstream


        snprintf(newNode->logData, sizeof(newNode->logData),
=======
 snprintf(newNode->logData, sizeof(newNode->logData),
>>>>>>> Stashed changes
=======
 snprintf(newNode->logData, sizeof(newNode->logData),
>>>>>>> Stashed changes
                 "Serra %d: %s\n  Temperatura: %d°C\n  Umidita': %d%%\n  Luce: %d\n  Umidita' terreno: %d\n  Livello acqua: %d\n  Orario: %02d:%02d\n  Stagione: %s\n-----------------------------\n",
                 i + 1, serre[i].pianta.nome, serre[i].temperatura, serre[i].umidita, serre[i].luce,
                 serre[i].umidita_terreno, serre[i].livello_acqua, serre[i].orario.tm_hour,
                 serre[i].orario.tm_min, determinaStagione(serre[i].orario.tm_mon + 1));

        newNode->next = NULL;

        if (head == NULL) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
<<<<<<< Updated upstream
<<<<<<< Updated upstream

    // Salvataggio dei dati dalla lista concatenata nel file
    LogNode* current = head;
    while (current != NULL) {
        fprintf(fileSalvataggio, "%s", current->logData);
        LogNode* temp = current;
        current = current->next;
        free(temp); // Liberazione della memoria del nodo
    }

    fclose(fileSalvataggio);
    printf("Log salvato con successo in %s\n", nomeFile);
}
=======
// aggiungi ricorsione
>>>>>>> Stashed changes
=======
// aggiungi ricorsione
>>>>>>> Stashed changes
