/* proclist.c */

/*
    Copyright (C) 2023  Maurice Lambert
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include  "proclist.h"
#include <stddef.h>
#include <stdlib.h>

/*
  * This function initializes the process list.
*/
void init_proc_list(StartProcList *list) {
    list->length = 0;
    list->first = NULL;
    list->last = NULL;
    list->position = NULL;
};

/*
  * This function free each processus and free the StartProcList memory.
*/
void clean_proc_list(StartProcList *list) {
    ProcessElementList *element = list->first;
    ProcessElementList *new_element;

    while (element != NULL) {
        new_element = element->next;
        free(element);
        element = new_element;
    }

    free(list);
}

/*
  * This function adds a process in the list.
*/
void add_proc(StartProcList *list, ProcessElementList *element) {
    if (list->last == NULL) {
        list->first = element;
        list->position = element;
    } else {
        list->last->next = element;
        element->precedent = list->last;
    }
    
    list->length += 1;
    list->last = element;
}

/*
  * This function returns and delete the last process.
  * This function returns NULL if last element is not defined.
*/
ProcessElementList *pop_proc(StartProcList *list) {
    if (list->last == NULL) {
        return NULL;
    }
    ProcessElementList *last = list->last;
    list->last = last->precedent;
    list->last->next = NULL;
    list->length -= 1;
    return last;
}

/*
  * This function returns and delete the first process.
  * This function returns NULL if the first process is not defined.
*/
ProcessElementList *popleft_proc(StartProcList *list) {
    if (list->first == NULL) {
        return NULL;
    }
    
    ProcessElementList *first = list->first;
    list->first = first->next;
    list->first->precedent = NULL;
    list->length -= 1;
    return first;
}

/*
  * This function inserts a process in specific position.
  * Return 1 if index is greater than list length.
*/
char insert_proc(StartProcList *list, ProcessElementList *new_element, unsigned int index) {
    if (index > list->length) {
        return 1;
    } else if (index == list->length) {
        add_proc(list, new_element);
    } else {
        ProcessElementList *element = list->first;
        for (unsigned int position = 0; index > position; position += 1) element = element->next;

        if (element->precedent != NULL) {
            element->precedent->next = new_element;
            new_element->precedent = element->precedent;
        } else {
        	list->first = new_element;
        }

        element->precedent = new_element;
        new_element->next = element;
        list->length += 1;
    }
    
    return 0;
}

/*
  * This function inserts a process after a specific process.
*/
void insert_after_proc(StartProcList *list, ProcessElementList *new_element, ProcessElementList *before) {
    new_element->next = before->next;
    new_element->precedent = before;
    
    if (before->next != NULL) {
        before->next->precedent = new_element;
    }

    before->next = new_element;
    list->length += 1;
}

/*
  * This function inserts a process before a specific process.
*/
void insert_before_proc(StartProcList *list, ProcessElementList *new_element, ProcessElementList *after) {
    new_element->precedent = after->precedent;
    new_element->next = after;

    if (after->precedent != NULL) {
        after->precedent->next = new_element;
    }

    after->precedent = new_element;
    list->length += 1;
}

/*
  * This function removes and free a process at a specific position.
  * This function returns 1 if index is greater or equal than list length.
*/
char remove_proc_index(StartProcList *list, unsigned int index) {
    if (list->length <= index) {
        return 1;
    } else if (index == (list->length - 1)) {
        ProcessElementList *process = list->last;
        
        if (list->last->precedent != NULL) {     // list last is not NULL because the precedent condition check the length is greater than 0
            list->last->precedent->next = NULL;
        }
        
        list->last = list->last->precedent;
        list->length -= 1;
        
        free(process);
        
        return 0;
    }
    
    ProcessElementList *element = list->first;
    for (unsigned int position = 0; index > position; position += 1) element = element->next;

    if (element->precedent != NULL) element->precedent->next = element->next;
    element->next->precedent = element->precedent;  // element next is not NULL because element is not the last element
    list->length -= 1;

    free(element);

    return 0;
}

/*
  * This function removes and free a specific process.
*/
void remove_proc(StartProcList *list, ProcessElementList *element) {
    if (element->next != NULL) element->next->precedent = element->precedent;
    if (element->precedent != NULL) element->precedent->next = element->next;
    list->length -= 1;
    free(element);
}

/*
  * This function returns the next process.
  * This function returns NULL if the list position is greater than the last process position.
*/
ProcessElementList *get_next_proc(StartProcList *list) {
    if (list->position == NULL) {
        return NULL;
    }
    ProcessElementList *process = list->position;
    list->position = process->next;
    return process;
}

/*
  * This function returns the precedent process.
  * This function returns NULL if the list position is smaller than the first process position.
*/
ProcessElementList *get_precedent_proc(StartProcList *list) {
    if (list->position == NULL) {
        return NULL;
    }
    
    ProcessElementList *process = list->position;
    list->position = process->precedent;
    return process;
}

/*
  * This function returns a process by index.
  * This function returns NULL if index is greater or equal than list length.
*/
ProcessElementList *get_proc(StartProcList *list, unsigned int index) {
    if (index >= list->length) {
        return NULL;
    }
    
    ProcessElementList *element = list->first;
    for (unsigned int position = 0; index > position; position += 1) {
        element = element->next;
    }

    return element;
}

/*
  * This function returns a process by PID.
  * This function returns NULL if PID is not found.
*/
ProcessElementList *get_proc_pid(StartProcList *list, unsigned int pid) {
    ProcessElementList *element = list->first;
    while (element != NULL && element->pid != pid) element = element->next;
    if (element == NULL) return NULL;
    return element;
}

/*
  * This function places list on the first position.
*/
void goto_first_position (StartProcList *list) {
    list->position = list->first;
}

/*
  * This function places list on the last position.
*/
void goto_last_position (StartProcList *list) {
    list->position = list->last;
}

