#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "ausiliari/sensoristica.h"
#include "ausiliari/lista.h"
#include <string.h>
#include "ausiliari/sort.h"
#include "ausiliari/attuatori.h"

int leggiIntero(const char* prompt) {
    char buffer[100];
    int valore;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue; // errore di input
        }

        // Controlla se l'input è un intero valido
        if (sscanf(buffer, "%d", &valore) == 1) {
            return valore;
        } else {
            printf("Input non valido. Inserisci un numero.\n");
        }
    }
}





void stampaSerre(SerraDati serre[], int n) {
    printf("\nStato delle serre:\n");
    for (int i = 0; i < n; i++) {
        printf("Serra %d: %s - Umidita' terreno: %d\n", i + 1, serre[i].pianta.nome, serre[i].umidita_terreno);
    }
    printf("\n");
}

void stampaSerret(SerraDati serre[], int n) { // stampa serra temperature
    printf("\nStato delle serre:\n");
    for (int i = 0; i < n; i++) {
        printf("Serra %d: %s - temperatura:  %d\n", i + 1, serre[i].pianta.nome, serre[i].temperatura);
    }
    printf("\n");
}

void salvaLog(SerraDati serre[], int n) {
    printf("Salva il log delle attivita' delle serre.\n");
    printf("Inserisci il nome del file in cui vuoi salvare il log: ");
    char nomeFile[100];
    scanf("%99s", nomeFile);

    FILE* fileSalvataggio = fopen(nomeFile, "w");
    if (!fileSalvataggio) {
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

        snprintf(newNode->logData, sizeof(newNode->logData),
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
void salvaLogRicorsivo(SerraDati serre[], int n) {
    char nomeFile[100];
    printf("Inserisci il nome del file per lo storico: ");
    scanf("%99s", nomeFile);

    FILE* fp = fopen(nomeFile, "w");
    if (!fp) {
        perror("Errore apertura file");
        return;
    }

    // Avvia la scrittura ricorsiva dallo 0
    salvaStoricoRec(serre, n, 0, fp);

    fclose(fp);
    printf("Storico salvato con successo in %s\n", nomeFile);
}

int main() {

    SerraDati serre[6] = {
        {0, 0, 0, 0, 0, {0}, {"Basilico", 800, 1000, 1}},
        {0, 0, 0, 0, 0, {0}, {"Peperoncino", 750, 950, 1}},
        {0, 0, 0, 0, 0, {0}, {"Cipolla", 720, 900, 1}},
        {0, 0, 0, 0, 0, {0}, {"Margherita", 700, 920, 0}},
        {0, 0, 0, 0, 0, {0}, {"Tulipano", 730, 940, 1}},
        {0, 0, 0, 0, 0, {0}, {"Dente di Leone", 710, 930, 0}}
    };

    srand(time(NULL));
    int scelta_categoria, scelta_serra, conteggio;
    char uscita[3];
    int scelta_ordinamento;
    while (1) {

        printf("\n Seleziona la categoria di serre:\n");
        printf("1) Piante Commestibili (Basilico, Peperoncino, Cipolla)\n");
        printf("2) Piante da Abbellimento (Margherite, Tulipani, Dente di Leone)\n");
        printf("3) Ordinamento... \n"); //selection sort

        printf("4) Applicare insetticida\n");
        printf("5) Salva il log delle attivita' delle serre\n");
        printf("6) Esci\n");
        scelta_categoria = leggiIntero("Scelta categoria: ");

        if (scelta_categoria == 6) {
            printf("Uscita dal programma.\n");
            return 0;
        }

        if (scelta_categoria == 5) {
            salvaLog(serre, 6);  // Salva il log delle serre
            continue;
        }

        if (scelta_categoria == 3) {
            printf("Come vuoi ordinare le serre?");
            scanf("%d",&scelta_ordinamento);
            switch (scelta_ordinamento) {
                    case 1:
                  // Aggiorna i dati delle serre prima dell'ordinamento
                    for (int i = 0; i < 6; i++) {
                        leggiSensori(&serre[i]);
                    }

                    printf("\n--- Prima dell'ordinamento ---\n");
                    stampaSerre(serre, 6);
                    selectionSort(serre, 6);
                    printf("\n--- Dopo l'ordinamento ---\n");
                    stampaSerre(serre, 6);
                    continue;


                case 2:
              // Aggiorna i dati delle serre prima dell'ordinamento
                    for (int i = 0; i < 6; i++) {
                        leggiSensori(&serre[i]);
                        }

                printf("\n--- Prima dell'ordinamento ---\n");
                stampaSerret(serre, 6);
                selectiontemp(serre, 6);
                printf("\n--- Dopo l'ordinamento ---\n");
                stampaSerret(serre, 6);
                continue;
            }
        }


       if (scelta_categoria == 4) {
           int numero_serre = 6;
           irrigazioneInsetticida(serre, &numero_serre);
           continue;
        }
        if (scelta_categoria == 7) {
            controllaTetto();
            continue;
        }


        if (scelta_categoria < 1 || scelta_categoria > 2) {
            printf("Scelta non valida! Riprova.\n");
            continue;
        }

        int start = (scelta_categoria == 1) ? 0 : 3;
        int end = (scelta_categoria == 1) ? 3 : 6;

        // Scegli una serra specifica
        printf("\nSeleziona una serra:\n");
        for (int i = start; i < end; i++) {
            printf("%d) %s\n", i - start + 1, serre[i].pianta.nome);
        }
        printf("%d) Torna al menu principale\n", end - start + 1);
        scelta_serra = leggiIntero("Scelta serra: ");

        if (scelta_serra == end - start + 1) {
            continue;  // Torna al menu principale
            continue;
        }

        if (scelta_serra < 1 || scelta_serra > (end - start)) {
            printf("Scelta non valida! Riprova.\n");
            continue;
        }

        // Monitorare la serra selezionata
        int serra_index = start + (scelta_serra - 1);
        conteggio = 0;
        controllaTetto();
        while (1) {
            leggiSensori(&serre[serra_index]);

            printf("\n--- Serra: %s ---\n", serre[serra_index].pianta.nome);
            printf("Temperatura: %d°C\n", serre[serra_index].temperatura);
            printf("Umidita': %d%%\n", serre[serra_index].umidita);
            printf("Luce: %d\n", serre[serra_index].luce);
            printf("Umidita' terreno: %d\n", serre[serra_index].umidita_terreno);
            printf("Livello acqua: %d\n", serre[serra_index].livello_acqua);
            printf("Orario: %02d:%02d\n", serre[serra_index].orario.tm_hour, serre[serra_index].orario.tm_min);
            printf("Stagione: %s\n", determinaStagione(serre[serra_index].orario.tm_mon + 1));
            controllaTetto();
             printf("Tetto: %s\n", tetto_aperto ? "Aperto" : "Chiuso"); // Visualizza lo stato del tetto

            controllaIrrigazione(serre[serra_index].umidita_terreno, serre[serra_index].pianta, serre[serra_index].orario);
            controllaLivelloAcqua(serre[serra_index].livello_acqua);
            controllaVentolaRaffreddamento(serre[serra_index].temperatura, serre[serra_index].orario);
            controllaIlluminazione(serre[serra_index].luce, serre[serra_index].orario);

            sleep(1);
            conteggio++;

            if (conteggio == 5) {
                printf("\nVuoi uscire dalla modalita' monitoraggio? (si/no): ");
                scanf("%2s", uscita);
                if (strcmp(uscita, "si") == 0) {
                    break;  // Torna al menu principale

                }
                conteggio = 0; // Reset conteggio per continuare

            }
        }
    }

    return 0;
}
