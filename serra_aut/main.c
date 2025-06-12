#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "ausiliari/sensoristica.h"
#include "ausiliari/lista.h"
#include <string.h>
#include "ausiliari/attuatori.h"
#include "ausiliari/terna.h"
#include "ausiliari/utils.h"


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

void salvaLog(SerraDati serre[], int n, bool monitorate[]) { // salva il log delle sole serre che l'utente sceglie di monitorare
    bool almeno_una = false;
    for (int i = 0; i < n; i++) {
        if (monitorate[i]) {
            almeno_una = true;
            break;
        }
    }

    if (!almeno_una) {
        printf(" Nessuna serra monitorata\n");
        return;
    }

    // Chiedo il nome del file
    printf("Inserisci il nome del file con cui vuoi salvare il log: ");
    char nomeFile[100];
    scanf("%99s", nomeFile);

    FILE* fileSalvataggio = fopen(nomeFile, "w");
    if (!fileSalvataggio) {
        printf("Errore: Impossibile creare il file di salvataggio.\n");
        return;
    }

    // Scrivi i log solo per le serre monitorate
    for (int i = 0; i < n; i++) {
        if (monitorate[i]) {
            leggiSensori(&serre[i], i);

            fprintf(fileSalvataggio, "--- MONITORAGGIO: %s ---\n", serre[i].pianta.nome);
            fprintf(fileSalvataggio, "Temperatura: %dC\n", serre[i].temperatura);
            fprintf(fileSalvataggio, "Umidita': %d%%\n", serre[i].umidita);
            fprintf(fileSalvataggio, "Luce: %d\n", serre[i].luce);
            fprintf(fileSalvataggio, "Umidita' terreno: %d\n", serre[i].umidita_terreno);
            fprintf(fileSalvataggio, "Livello acqua: %d\n", serre[i].livello_acqua);
            fprintf(fileSalvataggio, "Orario: %02d:%02d\n", serre[i].orario.tm_hour, serre[i].orario.tm_min);
            fprintf(fileSalvataggio, "Stagione: %s\n", determinaStagione(serre[i].orario.tm_mon + 1));
            fprintf(fileSalvataggio, "Tetto: %s\n", serre[i].tetto ? "Aperto" : "Chiuso");
            fprintf(fileSalvataggio, "%s\n", serre[i].tetto ? "Pioggia non rilevata" : "Pioggia rilevata");
            fprintf(fileSalvataggio, "[%s] Motore acqua: %s\n", serre[i].pianta.nome, serre[i].motore_acqua ? "ACCESO" : "SPENTO");
            fprintf(fileSalvataggio, "%s\n", serre[i].livello_acqua < 200 ? "Livello acqua basso! LED ROSSO ON Buzzer ON" : "Livello acqua sufficiente. LED VERDE ON");
            fprintf(fileSalvataggio, "Ventola raffreddamento: %d (Orario: %02d:%02d)\n", serre[i].ventola_raffreddamento, serre[i].orario.tm_hour, serre[i].orario.tm_min);
            fprintf(fileSalvataggio, "%s\n", serre[i].luce > 300 ? "Spegni la luce artificiale (Giorno)" : "Accendi la luce artificiale (Notte)");
            fprintf(fileSalvataggio, "Ventola riciclo aria: %d\n", serre[i].ventola_riciclo);



            // Salva anche nella lista log in memoria
            char logBuffer[512];

            snprintf(logBuffer, sizeof(logBuffer),"Serra %d: %s\n" "  Temperatura: %d°C\n" "  Umidità: %d%%\n" "  Luce: %d\n" "  Umidità terreno: %d\n" "  Livello acqua: %d\n"
            "  Orario: %02d:%02d\n" "  Stagione: %s\n" "  Ventola raffreddamento: %d\n"  "  Ventola riciclo: %d\n" "  LED: %s\n" "  Buzzer: %s\n" "  Tetto: %s\n"
                "-----------------------------\n",
                i + 1, serre[i].pianta.nome, serre[i].temperatura, serre[i].umidita,
                serre[i].luce, serre[i].umidita_terreno, serre[i].livello_acqua,
                serre[i].orario.tm_hour, serre[i].orario.tm_min,
                determinaStagione(serre[i].orario.tm_mon + 1),
                serre[i].ventola_raffreddamento,
                serre[i].ventola_riciclo,
                serre[i].livello_acqua < 200 ? "Livello acqua basso! ROSSO ON" : "Livello acqua sufficiente. LED VERDE ON",
                serre[i].buzzer ? "Acceso" : "Spento",
                serre[i].tetto ? "Aperto\nPioggia non rilevata" :"Chiuso\nPioggia rilevata" );
                add_log_entry(logBuffer);

        }
    }

    fclose(fileSalvataggio); // chiudo il file
    printf("Log salvato con successo in %s\n", nomeFile);
}


int main() {
    // inizializzazione dei dati delle serre
    SerraDati serre[6] = {
        {0, 0, 0, 0, 0, {0}, {"Basilico", 800, 1000, 1}},
        {0, 0, 0, 0, 0, {0}, {"Peperoncino", 750, 950, 1}},
        {0, 0, 0, 0, 0, {0}, {"Cipolla", 720, 900, 1}},
        {0, 0, 0, 0, 0, {0}, {"Margherita", 700, 920, 0}},
        {0, 0, 0, 0, 0, {0}, {"Tulipano", 730, 940, 0}},
        {0, 0, 0, 0, 0, {0}, {"Dente di Leone", 710, 930, 0}}
    };
    bool serre_monitorate[6] = {false}; // array booleano che ho inizializzato a false il cui contenuto si aggiorna appena una serra viene monitorata

    init_log_list(); // inizializzazione lista concatenata
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
        printf("6) Elimina tutti i log e libera la memoria\n");
        printf("7) Esci\n");

        scelta_categoria = leggiIntero("Scelta categoria: ");
        // opzioni di monitoraggio
        if (scelta_categoria == 7) {
            printf("Uscita dal programma.\n");
            return 0;
        }
        if (scelta_categoria == 6) {
        // Specifica la directory da cui eliminare i file .txt
        const char* dir_path = "D:\\dati_utente\\Desktop\\lab_informatica\\esame_lab_info_serra\\Lab_Serra";
        // Elimina tutti i file .txt dalla directory specificata
        eliminaFileTXT(dir_path);

        // Elimina tutti i log dalla lista e libera la memoria
        delete_all_logs();
        free_log_list();

        printf("Tutti i log sono stati eliminati e la memoria e' stata liberata.\n");
        continue;
    }
        if (scelta_categoria == 5) {
        // Verifica se almeno una serra è stata monitorata
        bool almeno_una = false;
        for (int i = 0; i < 6; i++) {
            if (serre_monitorate[i]) {
                almeno_una = true;
                break;
            }
    }

            if (!almeno_una) { // se non è stata monitorata alcuna serra
            printf("Nessuna serra monitorata!\n");
            } else {
            salvaLog(serre, 6, serre_monitorate);  //salvo nel log le informazioni delle sole serre monitorate
            }
            continue;
}


        // opzioni di ordinamento
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
                        controlloTemperatura(serre, i); // Controllo e correggo la temperatura
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
        // se la scelta della categoria risulta essere non valida stampo un messaggio "scelta non valida"

        if (scelta_categoria < 1 || scelta_categoria > 2) {
            printf("Scelta non valida! Riprova.\n");
            continue;
        }
        // selezione elementi della serra
        int start = (scelta_categoria == 1) ? 0 : 3; // se scelgo categoria 1 ho 3 possibilità di scelta: Basilico...
        int end = (scelta_categoria == 1) ? 3 : 6; // se scelgo categoria 2 ho 3 possibilità di scelta: tulipano, margherita...

        // Scegli una serra specifica
        printf("\nSeleziona una serra:\n");
        for (int i = start; i < end; i++) {
            printf("%d) %s\n", i - start + 1, serre[i].pianta.nome);
        }
        printf("%d) Torna al menu principale\n", end - start + 1);
        scelta_serra = leggiIntero("Scelta serra: ");

        if (scelta_serra == end - start + 1) {
            continue;  // Torna al menu principale
        }

        if (scelta_serra < 1 || scelta_serra > (end - start)) {
            printf("Scelta non valida! Riprova.\n");
            continue;
        }



       // monitoraggio della serra selezionata
        int serra_index = start + (scelta_serra - 1);
        char uscita_monitoraggio[4];


        // monitoraggio delle serre
        do{
            leggiSensori(&serre[serra_index], serra_index);
            controllaTetto(&serre[serra_index]);

            controlloTemperatura(serre, serra_index);
            int temp_iniziale = serre[serra_index].temperatura;
            int umidita_iniziale = serre[serra_index].umidita;
            int luce_iniziale = serre[serra_index].luce;
            int umidita_terreno_iniziale = serre[serra_index].umidita_terreno;
            int livello_acqua_iniziale = serre[serra_index].livello_acqua;
            // Dati iniziali di monitoraggio
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
            controllaVentolaRaffreddamento(serre[serra_index].temperatura, serre[serra_index].orario, &serre[serra_index]);
            controllaIlluminazione(serre[serra_index].luce, serre[serra_index].orario);
            controllaVentolaRiciclo(serre[serra_index].orario,&serre[serra_index]);
            printf("\nAttendere 60 secondi per il secondo monitoraggio...\n");

            int delta = rand() % 3 - 1; // variazione di +/- 1 dei dati

            serre[serra_index].temperatura = temp_iniziale + delta;
            serre[serra_index].umidita = umidita_iniziale + delta;
            serre[serra_index].luce = luce_iniziale + delta;
            serre[serra_index].umidita_terreno = umidita_terreno_iniziale + delta;
            serre[serra_index].livello_acqua = livello_acqua_iniziale + delta;

            //monitoraggio dopo 1 minuto

            sleep(60); //aspetto 60 secondi per effettuare il monitoraggio successivo
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
            controllaVentolaRaffreddamento(serre[serra_index].temperatura, serre[serra_index].orario, &serre[serra_index]);
            controllaIlluminazione(serre[serra_index].luce, serre[serra_index].orario);
            controllaVentolaRiciclo(serre[serra_index].orario,&serre[serra_index]);
            serre_monitorate[serra_index] = true;



            // chiedo all'utente se vuole tornare al menù principale
            printf("\nVuoi tornare al menu principale? (si/no): ");
            scanf("%3s", uscita_monitoraggio);

            }
    while (strcmp(uscita_monitoraggio, "no") == 0); // no= il monitoraggio continua fin quando l'utente non dice "si"
        }

    return 0;
}
