#include "lista.h"

LogNode* head = NULL;
LogNode* curN = NULL;

void init_log_list() {
    head = NULL;
    curN = NULL;
}

void print_log_list() {
    LogNode* current = head;
    printf("\n----- Registro Attivita' Serra -----\n");
    while (current != NULL) {
        printf("%s", current->logData);
        current = current->next;
    }
    printf("----- Fine Registro -----\n");
}

void add_log_entry(const char* logData) {
    LogNode* newNode = (LogNode*)malloc(sizeof(LogNode));
    if (newNode == NULL) {
        printf("Errore: Memoria insufficiente\n");
        return;
    }
    int lettura_nodo=0;
    strncpy(newNode->logData, logData, sizeof(newNode->logData) - 1);
    newNode->logData[sizeof(newNode->logData) - 1] = '\0';
    newNode->next = NULL;

    if (head == NULL) {
        head = newNode;
    } else {
        LogNode* current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
        printf("letto nodo %d", lettura_nodo);
    }
}

void free_log_list() {
    LogNode* current = head;
    while (current != NULL) {
        LogNode* nextNode = current->next;
        free(current);
        current = nextNode;
    }
    head = NULL;
    curN = NULL;
}

LogNode* delete_log_entry(const char* logData) {
    LogNode* previous = NULL;
    LogNode* current = head;

    while (current != NULL) {
        if (strcmp(current->logData, logData) == 0) {
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

bool is_log_list_empty() {
    return head == NULL;
}

int get_log_count() {
    int count = 0;
    LogNode* current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

void insert_log_after(LogNode* position, const char* logData) {
    if (position == NULL) {
        printf("Errore: Posizione non valida\n");
        return;
    }

    LogNode* newNode = (LogNode*)malloc(sizeof(LogNode));
    if (newNode == NULL) {
        printf("Errore: Memoria insufficiente\n");
        return;
    }

    strncpy(newNode->logData, logData, sizeof(newNode->logData) - 1);
    newNode->logData[sizeof(newNode->logData) - 1] = '\0';
    newNode->next = position->next;
    position->next = newNode;
}

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

void delete_all_logs() {
    free_log_list();
    printf("Tutti i log sono stati eliminati.\n");
}

bool hasNextNode() {
    return curN != NULL;
}

void rewindCursor() {
    curN = head;
}

LogNode* nextNode() {
    LogNode* temp = NULL;
    if (hasNextNode()) {
        temp = curN;
        curN = curN->next;
    }
    return temp;
}
