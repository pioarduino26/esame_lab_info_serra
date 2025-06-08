#include "terna.h"
#include "sensoristica.h"  // Includi il file che contiene la funzione di controllo

// Definizione dei valori fissi per le serre (temperatura e umidità del terreno)
ValoriFissi valori_fissi[] = {
    {21, 1000},   // Basilico
    {25, 950},    // Peperoncino
    {17, 900},    // Cipolla
    {20, 920},    // Margherita
    {20, 940},    // Tulipano
    {22, 930}     // Dente di Leone
}; //uso dei valori fissi più conveniente e meno laborioso rispetto all'uso di rand()



// Funzione di controllo della temperatura
void controlloTemperatura(SerraDati serre[], int n) {
        // Confronta la temperatura letta con quella fissa
        // controllo se la temperatura è fuori dal range accettabile +/- 5°C fissi
        if (serre[n].temperatura < (valori_fissi[n].temperatura_fissa - 5) ||
            serre[n].temperatura > (valori_fissi[n].temperatura_fissa + 5)) {
            printf("La temperatura della serra %d e'fuori range! Correggo il valore a...\n", n + 1);
            // Correggo la temperatura della serra riportandola al valore ideale
            serre[n].temperatura = valori_fissi[n].temperatura_fissa;
            printf("La temperatura della serra %d e' stata ripristinata a: %d C\n", n + 1, serre[n].temperatura);
        } else {
            printf("La temperatura della serra %d e' corretta ed e': %d C\n", n + 1, serre[n].temperatura);
        }
}
