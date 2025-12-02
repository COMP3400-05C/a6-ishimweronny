#include "parta.h"
#include <stdlib.h>
#include <stdio.h>

/**
* Helper function to get the minimum of two values
*/
int min(int a, int b) {
    return (a < b) ? a : b;
}

/**
 * Initializes each process based on given bursts as PCBs
 * @param bursts array of bursts to initialize as PCBs
 * @param blen length of bursts array
 */
struct pcb* init_procs(int* bursts, int blen) {
    // allocate memory on heap for procs
    struct pcb* procs = malloc(sizeof(struct pcb) * blen);

    // assign proper values to each PCB
    for (int i = 0; i < blen; i++) {
        procs[i].pid = i;
        procs[i].burst_left = bursts[i];
        procs[i].wait = 0;
    }

    return procs;
}

/**
 * Prints all PCBs in a struct
 * @param procs array of PCBs
 * @param plen length of procs array
 */
void printall(struct pcb* procs, int plen) {
    for (int i = 0; i < plen; i++) {
        printf("PCB %d\n", procs[i].pid);
        printf("---------------\n");
        printf("Burst remaining: %d\n", procs[i].burst_left);
        printf("Current wait: %d\n", procs[i].wait);
        printf("---------------\n\n");
    }
}

/**
 * "Runs" the current process by simulating CPU scheduling
 * @param procs the array of PCBs
 * @param plen the leangth of the procs array
 * @param current the PID to run
 * @param amount the amount to run the current process for
 */
void run_proc(struct pcb* procs, int plen, int current, int amount) {
    // decrement burst left, but first check if burst left is 0
    if (procs[current].burst_left == 0) {return;}
    procs[current].burst_left -= amount;

    // increase wait for all other procs while making proper checks
    for (int i = 0; i < plen; i++) {
        if (i == current) {continue;}
        if (procs[i].burst_left == 0) {continue;}
        procs[i].wait += amount;
    }
}

/**
 * Runs each process based on First-Come-First-Served
 * @param procs the array of PCBs
 * @param plen the leangth of the procs array
 */
int fcfs_run(struct pcb* procs, int plen) {
    int time = 0;

    // run each process in order (fcfs) and increment time
    for (int i = 0; i < plen; i++) {
        if (procs[i].burst_left == 0) {continue;}
        time += procs[i].burst_left;
        run_proc(procs, plen, i, procs[i].burst_left);
    }

    return time;
}

/**
 * Gets the next process to run for Round Robin scheduling
 * @param current the current process that just ran
 * @param procs the array of PCBs
 * @param plen the leangth of the procs array
 */
int rr_next(int current, struct pcb* procs, int plen) {
    // use modulo for wrap-around
    int next = (current + 1) % plen;

    for (int checked = 0; checked < plen; checked++) {
        if (procs[next].burst_left > 0) {return next;}
        next = (next + 1) % plen; // continue checking
    }

    return -1; // all processes are done if we're here
}

/**
 * Runs each process based on Round Robin scheduling
 * @param procs the array of PCBs
 * @param plen the leangth of the procs array
 * @param quantum the time allocated to each process in a round
 */
int rr_run(struct pcb* procs, int plen, int quantum) {
    int time = 0;

    // always run P0 first
    if (procs[0].burst_left > 0) {
        int run = min(quantum, procs[0].burst_left); // check how long to run for
        time += run;
        run_proc(procs, plen, 0, run);
    }

    // find next process
    int next = rr_next(0, procs, plen);

    // continue running processes until none need any more time
    while (next != -1) {
        int run = min(quantum, procs[next].burst_left);
        time += run;
        run_proc(procs, plen, next, run);
        next = rr_next(next, procs, plen);
    }

    return time;
}

