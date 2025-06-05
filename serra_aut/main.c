#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "ausiliari/sensoristica.h"
#include "ausiliari/lista.h"
#include <string.h>
#include "ausiliari/attuatori.h"
#include "ausiliari/terna.h"



int leggiIntero(const char* prompt) {
    char buffer[100];
    int valore;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue; // errore di input
        }


        if (sscanf(buffer, "%d", &valore) == 1) {
            return valore;
        } else {
            printf("Input non valido. Inserisci un numero valido.\n");
        }
    }
}


void stampaSerre(SerraDati serre[], int n) { // funzione per stampare in base all'umidità le serre
    printf("\nStato delle serre:\n");
    for (int i = 0; i < n; i++) {
        printf("Serra %d: %s - Umidita' terreno: %d\n", i + 1, serre[i].pianta.nome, serre[i].umidita_terreno);
    }
    printf("\n");
}

void stampaSerret(SerraDati serre[], int n) { // stampa serra in base alle temperature
    printf("\nStato delle serre:\n");
    for (int i = 0; i < n; i++) {
        printf("Serra %d: %s - Temperatura: %dC\n", i + 1, serre[i].pianta.nome, serre[i].temperatura);
    }
    printf("\n");
}

void salvaLog(SerraDati serre[], int n) { // funzione per creare e salvare i log di testo
    printf("Salva il log delle attivita' delle serre.\n");
    printf("Inserisci il nome del file con cui vuoi salvare il log: ");
    char nomeFile[100];
    scanf("%99s", nomeFile); // leggo al max 99 caratteri

    FILE* fileSalvataggio = fopen(nomeFile, "w"); //apro il file di salvataggio in modalità di scrittura
    if (!fileSalvataggio) {
        printf("Errore: Impossibile creare il file di salvataggio.\n");
        return;
    }

    // Genera e salva i dati delle serre nel file specificato
    // Definizione della struttura per la lista concatenata
    typedef struct LogNode {
        char logData[256];
        struct LogNode* next; // è un puntatore al nodo successivo
    } LogNode;

    // Creazione della lista concatenata temporanea
    LogNode* head = NULL; //nodo di testa della lista
    LogNode* tail = NULL; //nodo di coda della lista

    for (int i = 0; i < n; i++) {
        leggiSensori(&serre[i],i);  // Aggiorna i dati della serra
        // visualizzazione dei dei dati
        fprintf(fileSalvataggio, "Serra %d: %s\n", i + 1, serre[i].pianta.nome);
        fprintf(fileSalvataggio, "  Temperatura: %dÂ°C\n", serre[i].temperatura);
        fprintf(fileSalvataggio, "  Umidita': %d%%\n", serre[i].umidita);
        fprintf(fileSalvataggio, "  Luce: %d\n", serre[i].luce);
        fprintf(fileSalvataggio, "  Umidita' terreno: %d\n", serre[i].umidita_terreno);
        fprintf(fileSalvataggio, "  Livello acqua: %d\n", serre[i].livello_acqua);
        fprintf(fileSalvataggio, "  Orario: %02d:%02d\n", serre[i].orario.tm_hour, serre[i].orario.tm_min);
        fprintf(fileSalvataggio, "  Stagione: %s\n", determinaStagione(serre[i].orario.tm_mon + 1));
        fprintf(fileSalvataggio, "-----------------------------\n");

        LogNode* newNode = (LogNode*)malloc(sizeof(LogNode)); // alloco la memoria dinamicamente per un nuovo nodo di tipo LogNode e ne restituisco il puntatore
        if (newNode == NULL) { //se il puntatore al nuovo nodo è NULL
            printf("Errore: Memoria insufficiente per creare il nodo.\n");
            break;
        }

        snprintf(newNode->logData, sizeof(newNode->logData),
                 "Serra %d: %s\n  Temperatura: %dC\n  Umidita': %d%%\n  Luce: %d\n  Umidita' terreno: %d\n  Livello acqua: %d\n  Orario: %02d:%02d\n  Stagione: %s\n-----------------------------\n",
                 i + 1, serre[i].pianta.nome, serre[i].temperatura, serre[i].umidita, serre[i].luce,
                 serre[i].umidita_terreno, serre[i].livello_acqua, serre[i].orario.tm_hour,
                 serre[i].orario.tm_min, determinaStagione(serre[i].orario.tm_mon + 1)); // compongo una stringa con tutti i dati della serra e la memorizza in newNode->logData garantendo di non superare la dim del buffer

        newNode->next = NULL; // il nuovo nodo è l'ultimo della lista

        if (head == NULL) { // se la testa della lista è NULL (vuota)
            head = tail = newNode; //alla testa assegno coda e alla coda assegno newNode (nodo successivo)
        } else {
            tail->next = newNode; //la coda punta al nuovo nodo
            tail = newNode; // alla coda assegno il nuovo nodo
        }
    }

    // Salvataggio dei dati dalla lista concatenata nel file
    LogNode* current = head; // inizializzo un puntatore corrente al 1 nodo della lista
    while (current != NULL) { // fin tanto che ci sono nodi nella lista
        fprintf(fileSalvataggio, "%s", current->logData); //scrivo sul file il contenuto del nodo corrente
        LogNode* temp = current; // creo un nuovo puntatore a cui assegno i valori di current
        current = current->next; // punto il nodo corrente al successivo della lista
        free(temp); // libero la memoria del nodo
    }

    fclose(fileSalvataggio);
    printf("Log salvato con successo in %s\n", nomeFile);
}
    // ricorsione
    void salvaLogRicorsivo(SerraDati serre[], int n) {
    char nomeFile[100];
    printf("Inserisci il nome del file per lo storico: ");
    scanf("%99s", nomeFile);
    //apertura file in modalita' scrittura
    FILE* fp = fopen(nomeFile, "w");
    if (!fp) { // se il file non è stato aperto correttamente
        perror("Errore apertura file");
        return;
    }

    // Avvia la scrittura ricorsiva dallo 0
    salvaStoricoRec(serre, n, 0, fp);

    fclose(fp);
    printf("Storico salvato con successo in %s\n", nomeFile); // salvataggio storico serra
}

int main() {
    // inizializzazione dei dati delle serre
    SerraDati serre[6] = {
        {0, 0, 0, 0, 0, {0}, {"Basilico", 800, 1000, 1}},
        {0, 0, 0, 0, 0, {0}, {"Peperoncino", 750, 950, 1}},
        {0, 0, 0, 0, 0, {0}, {"Cipolla", 720, 900, 1}},
        {0, 0, 0, 0, 0, {0}, {"Margherita", 700, 920, 0}},
        {0, 0, 0, 0, 0, {0}, {"Tulipano", 730, 940, 1}},
        {0, 0, 0, 0, 0, {0}, {"Dente di Leone", 710, 930, 0}}
    };
    // generatore valori casuali
    srand(time(NULL));

    int scelta_categoria, scelta_serra, conteggio;
    char uscita[3];
    int scelta_ordinamento;
    // menù
    while (1) {

        printf("\n Seleziona la categoria di serre:\n");
        printf("1) Piante Commestibili (Basilico, Peperoncino, Cipolla)\n");
        printf("2) Piante da Abbellimento (Margherite, Tulipani, Dente di Leone)\n");
        printf("3) Ordinamento... \n"); //selection sort

        printf("4) Applicare insetticida\n");
        printf("5) Salva il log delle attivita' delle serre\n");
        printf("6) Esci\n");

        scelta_categoria = leggiIntero("Scelta categoria: ");
        // sotto vi sono le varie scelte
        if (scelta_categoria == 6) {
            printf("Uscita dal programma.\n");
            return 0;
        }

        if (scelta_categoria == 5) {
            salvaLog(serre, 6);  // Salva il log delle serre
            continue;
        }
        // ordinamento
        if (scelta_categoria == 3) {
            printf("Come vuoi ordinare le serre?");
            scanf("%d",&scelta_ordinamento);
            switch (scelta_ordinamento) {
                // ordinamento per umidità
                    case 1:
                  // Aggiorna i dati delle serre prima dell'ordinamento
                    for (int i = 0; i < 6; i++) {
                        leggiSensori(&serre[i],i); //invoco la funzione leggiSensori passando il riferimento alla serra corrente per leggere dati da sensori
                    }

                    printf("\n--- Prima dell'ordinamento ---\n");
                    stampaSerre(serre, 6);
                    selectionSort(serre, 6);
                    printf("\n--- Dopo l'ordinamento ---\n");
                    stampaSerre(serre, 6);
                    continue;

                //ordinamento per temperatura
                case 2:
              // Aggiorna i dati delle serre prima dell'ordinamento
                    for (int i = 0; i < 6; i++) {
                        leggiSensori(&serre[i],i); //invoco la funzione leggiSensori passando il riferimento alla serra corrente per leggere dati da sensori
                        }

                printf("\n--- Prima dell'ordinamento ---\n");
                stampaSerret(serre, 6);
                selectiontemp(serre, 6);
                printf("\n--- Dopo l'ordinamento ---\n");
                stampaSerret(serre, 6);
                continue;
            }
        }

        // applicazione dell'insetticida
       if (scelta_categoria == 4) {
           int numero_serre = 6;
           irrigazioneInsetticida(serre, &numero_serre); // invoco la funzione irrigazioneInsetticida per gestire l'insetticida sulle serre

           continue; // salto al prossimo ciclo
        }




        if (scelta_categoria < 1 || scelta_categoria > 2) {
            printf("Scelta non valida! Riprova.\n");
            continue;
        }
        // selezione elementi della serra
        int start = (scelta_categoria == 1) ? 0 : 3; // se selgo categoria 1 ho 3 possibilità di scelta Basilico....
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
        char uscita_monitoraggio[4];


        // monitoraggio delle serre
        do{
            leggiSensori(&serre[serra_index], serra_index);

            controlloTemperatura(serre, serra_index);

            int temp_iniziale = serre[serra_index].temperatura;
            int umidita_iniziale = serre[serra_index].umidita;
            int luce_iniziale = serre[serra_index].luce;
            int umidita_terreno_iniziale = serre[serra_index].umidita_terreno;
            int livello_acqua_iniziale = serre[serra_index].livello_acqua;
            // Dati iniziali
            printf("\n--- MONITORAGGIO: %s ---\n", serre[serra_index].pianta.nome);
            printf("Temperatura: %dC\n", temp_iniziale);
            printf("Umidita': %d%%\n", umidita_iniziale);
            printf("Luce: %d\n", luce_iniziale);
            printf("Umidita' terreno: %d\n", umidita_terreno_iniziale);
            printf("Livello acqua: %d\n", livello_acqua_iniziale);
            printf("Orario: %02d:%02d\n", serre[serra_index].orario.tm_hour, serre[serra_index].orario.tm_min);
            printf("Stagione: %s\n", determinaStagione(serre[serra_index].orario.tm_mon + 1));
            printf("Tetto: %s\n", tetto_aperto ? "Aperto \nPioggia non rilevata " : "Chiuso \nPioggia rilevata");
            controllaIrrigazione(serre[serra_index].umidita_terreno, serre[serra_index].pianta, serre[serra_index].orario);
            controllaLivelloAcqua(serre[serra_index].livello_acqua);
            controllaVentolaRaffreddamento(serre[serra_index].temperatura, serre[serra_index].orario);
            controllaIlluminazione(serre[serra_index].luce, serre[serra_index].orario);
            controllaVentolaRiciclo(serre[serra_index].orario);
            printf("\nAttendere 60 secondi per il secondo monitoraggio...\n");

            int delta = rand() % 3 - 1; // variazione di +/- 1 dei dati

            serre[serra_index].temperatura = temp_iniziale + delta;
            serre[serra_index].umidita = umidita_iniziale + delta;
            serre[serra_index].luce = luce_iniziale + delta;
            serre[serra_index].umidita_terreno = umidita_terreno_iniziale + delta;
            serre[serra_index].livello_acqua = livello_acqua_iniziale + delta;

            //monitoraggio dopo 1 minuto

            sleep(60); //aspetto 60 secondi
            // monitoraggio serra dopo 60 secondi
            printf("\n--- MONITORAGGIO DOPO 60 SECONDI: %s ---\n", serre[serra_index].pianta.nome);
            printf("Temperatura: %dC\n", temp_iniziale);
            printf("Umidita': %d%%\n", umidita_iniziale);
            printf("Luce: %d\n", luce_iniziale);
            printf("Umidita' terreno: %d\n", umidita_terreno_iniziale);
            printf("Livello acqua: %d\n", livello_acqua_iniziale);

            printf("Orario: %02d:%02d\n", serre[serra_index].orario.tm_hour, serre[serra_index].orario.tm_min+1);
            serre[serra_index].orario.tm_min += 1;
            if (serre[serra_index].orario.tm_min >= 60) {
                    serre[serra_index].orario.tm_min = 0;
                    serre[serra_index].orario.tm_hour += 1;
                        if (serre[serra_index].orario.tm_hour >= 24) {
                            serre[serra_index].orario.tm_hour = 0;
                            }
                        }
            printf("Stagione: %s\n", determinaStagione(serre[serra_index].orario.tm_mon + 1));
            printf("Tetto: %s\n", tetto_aperto ? "Aperto \nPioggia non rilevata" : "Chiuso \nPioggia rilevata");
            controllaIrrigazione(serre[serra_index].umidita_terreno, serre[serra_index].pianta, serre[serra_index].orario);
            controllaLivelloAcqua(serre[serra_index].livello_acqua);
            controllaVentolaRaffreddamento(serre[serra_index].temperatura, serre[serra_index].orario);
            controllaIlluminazione(serre[serra_index].luce, serre[serra_index].orario);
            controllaVentolaRiciclo(serre[serra_index].orario);



            // chiedo all'utente se vuole tornare al menù principale
            printf("\nVuoi tornare al menu principale? (si/no): ");
            scanf("%3s", uscita_monitoraggio);

            }
    while (strcmp(uscita_monitoraggio, "no") == 0); // no= il monitoraggio continua fin quando l'utente non dice "si"
        }

    return 0;
}
