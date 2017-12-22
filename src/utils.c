#include <stddef.h>
#include "utils.h"

void addLast(Name* list, Name* new) {
    Name* on = list;
    while (on->next != NULL) on = on->next;
    on->next = new;
}

Name* popFirst(Name** list) {
    if (list == NULL) return NULL;
    Name* first = *list;
    list = &(*list)->next;
    return first;
    //first->addLast()
}
