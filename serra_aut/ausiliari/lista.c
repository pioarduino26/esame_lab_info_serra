#include "lista.h"
// in lista.h viene definita la struutura LogNode
LogNode* head = NULL; //inizializzo il puntatore al primo nodo della lista
LogNode* curN = NULL; //inizializzo a NULL il cursore per scorrere la lista

void init_log_list() { //inizializzazione della lista (impostandola vuota)
    head = NULL;
    curN = NULL;
}

void print_log_list() { //(stampo tutti i log della lista partendo dalla testa)
    LogNode* current = head;
    printf("\n----- Registro Attivita' Serra -----\n");

    while (current != NULL) { // scorro tutta la lista
        printf("%s", current->logData); //stampo il contenuto del log
        current = current->next; // passo al nodo successivo
    }
    printf("----- Fine Registro -----\n");
}

//creo un nuovo nodo con la malloc
void add_log_entry(const char* logData) {
    LogNode* newNode = (LogNode*)malloc(sizeof(LogNode));
    if (newNode == NULL) { // controllo che la memoria sia allocata correttamente
        printf("Errore: Memoria insufficiente\n");
        return;
    }
    //copio la stringa logData nel nuovo nodo terminando con \0
    int lettura_nodo=0;
    strncpy(newNode->logData, logData, sizeof(newNode->logData) - 1);
    newNode->logData[sizeof(newNode->logData) - 1] = '\0';
    newNode->next = NULL; // faccio puntare il nuovo nodo a fine lista
    //inserisco il nuovo nodo in lista
    if (head == NULL) { //lista vuota
        head = newNode;  // il nodo nuovo diventa la testa della lista
    } else { // altrimenti vado a fine lista collegandolo all'ultimo nodo
        LogNode* current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
        printf("letto nodo %d", lettura_nodo);
    }
}
// libero la memoria di tutti i nodi
void free_log_list() {
    LogNode* current = head;
    while (current != NULL) { //libero ogni nodo e avanzo al successivo
        LogNode* nextNode = current->next;
        free(current);
        current = nextNode;
    }
    //azzero i puntatori locali
    head = NULL;
    curN = NULL;
}
//cerco un log da eliminare confrontando il contenuto (logData)
LogNode* delete_log_entry(const char* logData) {
    LogNode* previous = NULL;
    LogNode* current = head;

    while (current != NULL) {
        if (strcmp(current->logData, logData) == 0) {
                //Se trova una corrispondenza esatta della stringa:


            if (previous == NULL) {
                head = current->next;
            } else {
                previous->next = current->next;
            }
            return current;
        }
        previous = current;
        current = current->next;
    }
    return NULL;
}
// rimuove il nodo dalla lista
//Il nodo viene restituito ma non viene liberata la memoria

bool is_log_list_empty() { //se la lista è vuota la funzione ritorna true
    return head == NULL;
}
// conta e ritorna il numero di log nella lista
int get_log_count() {
    int count = 0;
    LogNode* current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}
// inserisco il log in un nodo specifico
void insert_log_after(LogNode* position, const char* logData) {
    if (position == NULL) {
        printf("Errore: Posizione non valida\n");
        return;
    }
    // alloco il nuovo nodo controllando la memoria
    LogNode* newNode = (LogNode*)malloc(sizeof(LogNode));
    if (newNode == NULL) {
        printf("Errore: Memoria insufficiente\n");
        return;
    }
    // inserisco il nuovo nodo nella posizione desiderata
    strncpy(newNode->logData, logData, sizeof(newNode->logData) - 1);
    newNode->logData[sizeof(newNode->logData) - 1] = '\0';
    newNode->next = position->next;
    position->next = newNode;
}
//Cerca un nodo che contiene una sottostringa all’interno del suo logData
LogNode* find_log_by_content(const char* content) {
    LogNode* current = head;
    while (current != NULL) {
        if (strstr(current->logData, content) != NULL) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}
//Elimina tutti i log e stampa un messaggio di conferma.
void delete_all_logs() {
    free_log_list();
    printf("Tutti i log sono stati eliminati.\n");
}
// Verifica se ci sono ancora nodi da leggere con il cursore curN
bool hasNextNode() {
    return curN != NULL;
}
//Riporta il cursore all'inizio della lista (utile per iterazioni manuali).
void rewindCursor() {
    curN = head;
}
//Restituisce il nodo corrente e sposta curN al nodo successivo (iteratore "manuale").
LogNode* nextNode() {
    LogNode* temp = NULL;
    if (hasNextNode()) {
        temp = curN;
        curN = curN->next;
    }
    return temp;
}
