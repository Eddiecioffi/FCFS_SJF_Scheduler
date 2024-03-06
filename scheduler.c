// This program simulates two CPU scheduling algorithms: First-Come-First-Serve (FCFS) and Shortest Job First (SJF)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to hold process information
struct Process {
    int id;                 // Process ID
    int arrival_time;       // Arrival time of the process
    int burst_time;         // Burst time of the process
    int waiting_time;       // Time the process waits in the ready queue
    int turnaround_time;    // Time taken for the process to execute
};

// Swap between two process structures
void swap(struct Process* a, struct Process* b) {
    struct Process temp = *a;
    *a = *b;
    *b = temp;
}

// Perform FCFS scheduling
void FCFS(struct Process* processes, int num_processes) {
    int current_time = 0;   // Current time in the system

    for (int i = 0; i < num_processes; i++) {
        // If the process hasn't arrived yet, wait until it does
        if (processes[i].arrival_time > current_time) {
            current_time = processes[i].arrival_time;
        }

        // Calculate waiting time and turnaround time for the process
        processes[i].waiting_time = current_time - processes[i].arrival_time;
        processes[i].turnaround_time = processes[i].waiting_time + processes[i].burst_time;

        // Update current time to reflect the completion of this process
        current_time += processes[i].burst_time;
    }
}

// Perform SJF scheduling
void SJF(struct Process* processes, int num_processes) {
    int current_time = 0;   // Current time in the system

    // Sort processes by burst time (shortest first)
    for (int i = 0; i < num_processes - 1; i++) {
        for (int j = 0; j < num_processes - i - 1; j++) {
            if (processes[j].burst_time > processes[j + 1].burst_time) {
                swap(&processes[j], &processes[j + 1]);
            }
        }
    }

    for (int i = 0; i < num_processes; i++) {
        // If the process hasn't arrived yet, wait until it does
        if (processes[i].arrival_time > current_time) {
            current_time = processes[i].arrival_time;
        }

        processes[i].waiting_time = current_time - processes[i].arrival_time;
        processes[i].turnaround_time = processes[i].waiting_time + processes[i].burst_time;

        // Update current time to reflect the completion of this process
        current_time += processes[i].burst_time;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <input_file> <scheduling_algorithm (FCFS/SJF)>\n", argv[0]);
        return 1;
    }

    // Open the input file
    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    int num_processes;
    fscanf(file, "%d", &num_processes);

    // Allocate memory for storing process information
    struct Process* processes = malloc(num_processes * sizeof(struct Process));

    // Read process information from the file
    for (int i = 0; i < num_processes; i++) {
        processes[i].id = i + 1; // Assigning IDs starting from 1
        fscanf(file, "%d %d", &processes[i].arrival_time, &processes[i].burst_time);
    }

    fclose(file);

    // Determine which scheduling algorithm to use
    if (strcmp(argv[2], "FCFS") == 0) {
        FCFS(processes, num_processes);
    } else if (strcmp(argv[2], "SJF") == 0) {
        SJF(processes, num_processes);
    } else {
        printf("Invalid scheduling algorithm specified.\n");
        return 1;
    }

    // Calculate average waiting time/average turnaround time
    float avg_waiting_time = 0;
    float avg_turnaround_time = 0;

    for (int i = 0; i < num_processes; i++) {
        avg_waiting_time += processes[i].waiting_time;
        avg_turnaround_time += processes[i].turnaround_time;
    }

    avg_waiting_time /= num_processes;
    avg_turnaround_time /= num_processes;

    // Print scheduling order and average times
    printf("Scheduling order: ");
    for (int i = 0; i < num_processes; i++) {
        printf("P%d", processes[i].id);
        if (i < num_processes - 1) {
            printf(" -> ");
        }
    }
    printf("\n");

    printf("Average Waiting Time: %.2f\n", avg_waiting_time);
    printf("Average Turnaround Time: %.2f\n", avg_turnaround_time);

    // Free allocated memory
    free(processes);

    return 0;
}