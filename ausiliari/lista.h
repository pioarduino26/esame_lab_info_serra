#ifndef LISTA_CONCATENATA_H
#define LISTA_CONCATENATA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "sensoristica.h"

typedef struct LogNode {
    char logData[512];
    struct LogNode* next;
} LogNode;

// Prototipi delle funzioni
void init_log_list();
void print_log_list();
void add_log_entry(const char* logData);
void free_log_list();
LogNode* delete_log_entry(const char* logData);
bool is_log_list_empty();
int get_log_count();
void insert_log_after(LogNode* position, const char* logData);
LogNode* find_log_by_content(const char* content);
void delete_all_logs();

#endif // LISTA_CONCATENATA_H
