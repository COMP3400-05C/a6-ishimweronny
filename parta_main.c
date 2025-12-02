#include "parta.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    // check min arguments
    if (argc < 3) {
        printf("ERROR: Missing arguments\n");
        return 1;
    }
    
    char* algorithm = argv[1];
    
    if (strcmp(algorithm, "fcfs") == 0) {
        // fcfs needs at least one burst value
        if (argc < 3) {
            printf("ERROR: Missing arguments\n");
            return 1;
        }
        
        int blen = argc - 2;
        int* bursts = (int*)malloc(blen * sizeof(int)); // malloc for bursts array
        
        printf("Using FCFS\n\n");
        
        // parse and print bursts
        for (int i = 0; i < blen; i++) {
            bursts[i] = atoi(argv[i + 2]); // use atoi to parse
            printf("Accepted P%d: Burst %d\n", i, bursts[i]);
        }
        
        // initialize and run scheduler
        struct pcb* procs = init_procs(bursts, blen);
        fcfs_run(procs, blen);
        
        // calculate average wait time
        int total_wait = 0;
        for (int i = 0; i < blen; i++) {
            total_wait += procs[i].wait;
        }
        double avg_wait = (double)total_wait / blen;
        
        printf("Average wait time: %.2f\n", avg_wait);
        
        free(bursts);
        free(procs);
        
    } else if (strcmp(algorithm, "rr") == 0) {
        // rr needs quantum and one burst value at least
        if (argc < 4) {
            printf("ERROR: Missing arguments\n");
            return 1;
        }
        
        int quantum = atoi(argv[2]);
        int blen = argc - 3;
        int* bursts = (int*)malloc(blen * sizeof(int));
        
        printf("Using RR(%d)\n\n", quantum);
        
        // parse and print bursts
        for (int i = 0; i < blen; i++) {
            bursts[i] = atoi(argv[i + 3]);
            printf("Accepted P%d: Burst %d\n", i, bursts[i]);
        }
        
        // initialize and run scheduler
        struct pcb* procs = init_procs(bursts, blen);
        rr_run(procs, blen, quantum);
        
        // calculate average wait time
        int total_wait = 0;
        for (int i = 0; i < blen; i++) {
            total_wait += procs[i].wait;
        }
        double avg_wait = (double)total_wait / blen;
        
        printf("Average wait time: %.2f\n", avg_wait);
        
        // free mem
        free(bursts);
        free(procs);
        
    } else {
        printf("ERROR: Missing arguments\n");
        return 1;
    }
    
    return 0;
}