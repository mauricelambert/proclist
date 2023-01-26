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
#include <stdio.h>

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

/*
  * This function is used for tests and prints an ordered PID list.
*/

void printPIDs(StartProcList *list) {
    printf("Length: %i\n", list->length);
    ProcessElementList *element = list->first;
    while (element != NULL) {
        printf("%i -> ", element->pid);
        element = element->next;
    }
    puts("");
    
    element = list->last;
    while (element != NULL) {
        printf("%i -> ", element->pid);
        element = element->precedent;
    }
    puts("");
}

/*
  * This function is used for tests and checks the list values.
*/

char check (StartProcList *list) {
    if (list->first->pid != 0) {
        printf("Error in insert_proc, first PID: %i\n", list->first->pid);
        return 2;
    }
    
    if (list->first->next->pid != 1) {
        printf("Error in insert_proc, second (first next) PID: %i\n", list->first->next->pid);
        return 2;
    }
    
    if (list->first->next->precedent->pid != 0) {
        printf("Error in insert_proc, second (first next precedent) PID: %i\n", list->first->next->precedent->pid);
        return 2;
    }
    
    if (list->first->next->next->pid != 2) {
        printf("Error in insert_proc, second (first next next) PID: %i\n", list->first->next->next->pid);
        return 2;
    }
    
    if (list->last->precedent->pid != 1) {
        printf("Error in insert_proc, second (last precedent) PID: %i\n", list->last->precedent->pid);
        return 2;
    }
    
    if (list->last->precedent->next->pid != 2) {
        printf("Error in insert_proc, second (last precedent next) PID: %i\n", list->last->precedent->next->pid);
        return 2;
    }
    
    if (list->last->precedent->precedent->pid != 0) {
        printf("Error in insert_proc, second (last precedent precedent) PID: %i\n", list->last->precedent->precedent->pid);
        return 2;
    }
    
    if (list->last->pid != 2) {
        printf("Error in insert_proc, last PID: %i\n", list->last->pid);
        return 2;
    }
    
    printf("%i -> %i -> %i\n", list->first->pid, list->first->next->pid, list->first->next->next->pid);
    printf("%i <- %i <- %i\n", list->last->precedent->precedent->pid, list->last->precedent->pid, list->last->pid);
    return 0;
}

/*
  * Main function to test my process list.
*/
int main(int argc, char *argv[]) {
    StartProcList *list = malloc(sizeof(StartProcList));
    
    if (list == NULL) {
        puts("malloc failed");
        return 1;
    }
    
    init_proc_list(list);
    
    ProcessElementList *process = pop_proc(list);
    
    if (process != NULL) {
        puts("pop_proc doesn't returns NULL");
        return 6;
    }
    
    process = popleft_proc(list);
    
    if (process != NULL) {
        puts("popleft_proc doesn't returns NULL");
        return 9;
    }
    
    ProcessElementList *first_process = malloc(sizeof(ProcessElementList));

    if (first_process == NULL) {
        puts("malloc failed");
        return 1;
    }
    
    first_process->pid = 0;
    first_process->ppid = 0;
    first_process->start_timestamp = 1466607358.0;
    
    first_process->tty = 0;
    first_process->cpu_usage = 0.1;
    first_process->memory_usage = 0.1;
    
    first_process->executable = "systemd";
    first_process->cmdline = "/usr/bin/systemd";
    first_process->user = "root";
    
    char code = insert_proc(list, first_process, 0);
    
    if (code != 0) {
        printf("Error %i in insert_proc\n", code);
        return 3;
    }
    
    ProcessElementList *third_process = malloc(sizeof(ProcessElementList));

    if (third_process == NULL) {
        puts("malloc failed");
        return 1;
    }
    
    third_process->pid = 2;
    third_process->ppid = 1;
    third_process->start_timestamp = 1466607358.0;
    
    third_process->tty = 1;
    third_process->cpu_usage = 0.7;
    third_process->memory_usage = 2;
    
    third_process->executable = "apache2";
    third_process->cmdline = "/usr/bin/apache2";
    third_process->user = "root";
    
    add_proc(list, third_process);
    
    ProcessElementList *second_process = malloc(sizeof(ProcessElementList));

    if (second_process == NULL) {
        puts("malloc failed");
        return 1;
    }
    
    second_process->pid = 1;
    second_process->ppid = 0;
    second_process->start_timestamp = 1466607358.0;
    
    second_process->tty = 0;
    second_process->cpu_usage = 0.0;
    second_process->memory_usage = 0.2;
    
    second_process->executable = "crontab";
    second_process->cmdline = "/usr/bin/crontab";
    second_process->user = "root";
    
    code = insert_proc(list, second_process, 1);
    
    if (code != 0) {
        printf("Error %i in insert_proc\n", code);
        return 4;
    }
    
    code = check(list);
    if (code) return code;
    printPIDs(list);
    
    code = insert_proc(list, second_process, 100);
    
    if (code == 0) {
        puts("No error in insert_proc");
        return 5;
    }
    
    ProcessElementList *delete_process = malloc(sizeof(ProcessElementList));

    if (delete_process == NULL) {
        puts("malloc failed");
        return 1;
    }
    
    delete_process->pid = 127;
    
    int length = list->length;
    ProcessElementList *delete_process1 = malloc(sizeof(ProcessElementList));

    if (delete_process1 == NULL) {
        puts("malloc failed");
        return 1;
    }
    
    delete_process1->pid = 126;
    
    insert_after_proc(list, delete_process1, delete_process);
    
    if (delete_process->next->pid != 126 || delete_process1->precedent->pid != 127) {
        printf("Error in insert_after_proc: next pid %i and precedent pid %i", delete_process->next->pid, delete_process1->precedent->pid);
        return 12;
    }
    
    delete_process->next = NULL;
    delete_process1->precedent = NULL;
    
    insert_before_proc(list, delete_process1, delete_process);
    
    if (delete_process->precedent->pid != 126 || delete_process1->next->pid != 127) {
        printf("Error in insert_before_proc: precedent pid %i and newt pid %i", delete_process->precedent->pid, delete_process1->next->pid);
        return 12;
    }
    
    delete_process->precedent = NULL;
    delete_process1->next = NULL;
    list->length = length;
    
    code = check(list);
    if (code) return code;
    printPIDs(list);
    
    add_proc(list, delete_process);
    code = insert_proc(list, delete_process1, 0);
    
    if (code != 0) {
        puts("Error in insert_proc (error code)");
        return 12;
    }
    
    if (list->length != 5) {
        printf("Error in add_proc or insert_proc: list length should be 5 (%i)\n", list->length);
        return 32;
    }
    
    if (list->first->pid != 126) {
        printf("Error in insert_proc: first PID should be 126 (%i)\n", list->first->pid);
        return 33;
    }
    
    if (list->first->next->pid != 0) {
        printf("Error in insert_proc: first next PID should be 0 (%i)\n", list->first->next->pid);
        return 33;
    }
    
    if (list->first->next->precedent->pid != 126) {
        printf("Error in insert_proc: first next precedent PID should be 126 (%i)\n", list->first->next->precedent->pid);
        return 33;
    }
    
    if (list->last->pid != 127) {
        printf("Error in add_proc: last PID should be 127 (%i)\n", list->first->pid);
        return 34;
    }
    
    if (list->last->precedent->pid != 2) {
        printf("Error in add_proc: last precedent PID should be 0 (%i)\n", list->first->precedent->pid);
        return 34;
    }
    
    if (list->last->precedent->next->pid != 127) {
        printf("Error in add_proc: last precedent next PID should be 127 (%i)\n", list->first->precedent->next->pid);
        return 34;
    }
    
    printPIDs(list);
    puts("1");
    
    process = pop_proc(list);
    printPIDs(list);
    
    if (process == NULL) {
        puts("Error in pop_proc, returns NULL.");
        return 7;
    }
    
    if (process->pid != 127) {
        printf("Error in pop_proc, returns a process with PID different than 1 (%i).\n", process->pid);
        return 8;
    }
    
    process = popleft_proc(list);
    printPIDs(list);
    
    if (process == NULL) {
        puts("Error in popleft_proc, returns NULL.");
        return 10;
    }
    
    if (process->pid != 126) {
        printf("Error in popleft_proc, returns a process with PID different than 126 (%i).\n", process->pid);
        return 11;
    }
    
    process = get_next_proc(list);
    printPIDs(list);
    puts("2");
    
    if (process == NULL) {
        puts("Error in get_next_proc, return value is NULL");
        return 13;
    }
    
    if (process->pid != 0) {
        printf("Error in get_next_proc, return process is not the first process (pid: %i)\n", process->pid);
        return 14;
    }
    
    process = get_precedent_proc(list);
    printPIDs(list);
    
    if (process == NULL) {
        puts("Error in get_precedent_proc, return value is NULL");
        return 15;
    }
    
    if (process->pid != 1) {
        printf("Error in get_precedent_proc, return process is not the second process (pid: %i)\n", process->pid);
        return 16;
    }
    
    process = get_precedent_proc(list);
    printPIDs(list);
    
    if (process == NULL) {
        puts("Error2 in get_precedent_proc, return value is NULL");
        return 15;
    }
    
    if (process->pid != 0) {
        printf("Error2 in get_precedent_proc, return process is not the first process (pid: %i)\n", process->pid);
        return 16;
    }
    
    process = get_precedent_proc(list);
    printPIDs(list);
    puts("3");
    
    if (process != NULL) {
        puts("Error in get_precedent_proc, return value is not NULL");
        return 17;
    }
    
    goto_first_position(list);
    printPIDs(list);
    
    process = get_precedent_proc(list);
    printPIDs(list);
    
    if (process == NULL) {
        puts("Error in goto_first_position or get_precedent_proc, return value is NULL");
        return 18;
    }
    
    if (process->pid != 0) {
        printf("Error in goto_first_position or get_precedent_proc, return process is not the first process (pid: %i)\n", process->pid);
        return 19;
    }
    
    goto_last_position(list);
    printPIDs(list);
    puts("4");
    
    process = get_next_proc(list);
    printPIDs(list);
    
    if (process == NULL) {
        puts("Error in goto_last_position or get_next_proc, return value is NULL");
        return 20;
    }
    
    if (process->pid != 2) {
        printf("Error in goto_last_position or get_next_proc, return process is not the third process (pid: %i)\n", process->pid);
        return 21;
    }
    
    process = get_next_proc(list);
    printPIDs(list);
    
    if (process != NULL) {
        puts("Error in get_next_proc, return value is not NULL");
        return 22;
    }
    
    process = get_proc(list, 100);
    printPIDs(list);
    puts("5");
    
    if (process != NULL) {
        puts("Error in get_proc, return value is not NULL");
        return 23;
    }
    
    process = get_proc(list, 1);
    printPIDs(list);
    
    if (process->pid != 1) {
        printf("Error in get_proc return value is not second process (pid: %i)\n", process->pid);
        return 29;
    }
    
    process = get_proc_pid(list, 100);
    printPIDs(list);
    
    if (process != NULL) {
        puts("Error in get_proc_pid, return value is not NULL");
        return 24;
    }
    
    process = get_proc_pid(list, 1);
    printPIDs(list);
    puts("6");
    
    if (process == NULL) {
        puts("Error in get_proc_pid, return value is NULL");
        return 26;
    }
    
    if (process->pid != 1) {
        printf("Error in get_proc_pid, return value is not second process (pid: %i)\n", process->pid);
        return 30;
    }
    
    remove_proc(list, second_process);
    printPIDs(list);
    
    if (first_process->next->pid != third_process->pid && third_process->precedent->pid != first_process->pid) {
        printf("Error in remove_proc first_process.next(%p != %p) third_process.precedent(%p != %p).\n", first_process->next, third_process, third_process->precedent, first_process);
        return 27;
    }
    
    ProcessElementList *delete_process2 = malloc(sizeof(ProcessElementList));

    if (delete_process1 == NULL) {
        puts("malloc failed");
        return 1;
    }
    
    delete_process2->pid = 125;
    
    insert_after_proc(list, delete_process2, first_process);
    printPIDs(list);

    remove_proc_index(list, 1);
    printPIDs(list);
    
    if (first_process->next->pid != third_process->pid && third_process->precedent->pid != first_process->pid) {
        printf("Error in remove_proc_index or insert_after_proc first_process.next(%p != %p) third_process.precedent(%p != %p).\n", first_process->next, third_process, third_process->precedent, first_process);
        return 28;
    }
    
    remove_proc_index(list, 1);
    printPIDs(list);
    
    if (first_process->next != NULL) {
        puts("Error in remove_proc_index first_process.next is not NULL.");
        return 31;
    }
    
    clean_proc_list(list);
    
    return 0;
}
