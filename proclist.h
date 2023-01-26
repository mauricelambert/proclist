/* proclist.h */

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

typedef struct ProcessElementList {
    struct ProcessElementList *next;
    struct ProcessElementList *precedent;

    char tty;                        // 0: process haven't tty; 1: process have tty
    float cpu_usage;
    float memory_usage;

    char *executable;
    char *cmdline;
    char *user;

    unsigned int pid;
    unsigned int ppid;

    long double start_timestamp;
} ProcessElementList;

typedef struct StartProcList {
    unsigned int length;
    ProcessElementList *first;
    ProcessElementList *last;
    ProcessElementList *position;
} StartProcList;

void init_proc_list(StartProcList *list);
void clean_proc_list(StartProcList *list);

void add_proc(StartProcList *list, ProcessElementList *element);
ProcessElementList *pop_proc(StartProcList *list);
ProcessElementList *popleft_proc(StartProcList *list);

char insert_proc(StartProcList *list, ProcessElementList *element, unsigned int index);
void insert_after_proc(StartProcList *list, ProcessElementList *element, ProcessElementList *before);
void insert_before_proc(StartProcList *list, ProcessElementList *element, ProcessElementList *after);

char remove_proc_index(StartProcList *list, unsigned int index);
void remove_proc(StartProcList *list, ProcessElementList *element);

ProcessElementList *get_next_proc(StartProcList *list);
ProcessElementList *get_precedent_proc(StartProcList *list);
ProcessElementList *get_proc(StartProcList *list, unsigned int index);
ProcessElementList *get_proc_pid(StartProcList *list, unsigned int pid);

void goto_first_position (StartProcList *list);
void goto_last_position (StartProcList *list);
