/* hcu.c: program for a custom waybar module to show the process with the
 * highest cpu usage. Homepage: https://github.com/trollLemon/highest-cpu-usage
 *
 * Copyright (C) 2023 Haydn Tamura (trolllemon)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <dirent.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/times.h>
#include <unistd.h>

/*
 *  :::::::::::::ProgramData:::::::::::::::
 *  This struct will hold relevant program data.
 *  Pid: this is the process id.
 *  percentage: this is the percentage of the CPU the process is using.
 *  process_name: array of chars for process names.
 *
 */
typedef struct {
    unsigned int pid;
    unsigned long long utime;
    unsigned long long stime;
    float usage;
    char process_name[256];
} ProgramData;

/*
 * Print help text
 */
void PrintHelp () {}

void _alloc_array (ProgramData** procs, int cap) {

    for (int i = 0; i < cap; i++) {
        procs[i] = malloc (sizeof (ProgramData));
        if (procs[i] == NULL) {
            printf ("Memory allocation failed.\n");
            for (int j = 0; j < i; j++) {
                free (procs[j]);
            }
        }
    }
}

unsigned long long _getTotalCpuTime () {

    FILE* file = fopen ("/proc/stat", "r");
    if (file == NULL) {
        perror ("Could not open stat file");
        return 0;
    }

    unsigned long long data[8];

    fscanf (file, "cpu  %llu %llu %llu %llu %llu %llu %llu %llu", &data[0],
            &data[1], &data[2], &data[3], &data[4], &data[5], &data[6],
            &data[7]);

    unsigned long long time = 0;

    for (int i = 0; i < 8; ++i) {
        time += data[i];
    }

    fclose (file);

    return time;
}

ProgramData** read_proc (ProgramData** procs, int* size, int* capacity) {

    DIR* dir = opendir ("/proc");

    struct dirent* dirData;
    while ((dirData = readdir (dir)) != NULL) {
        if (dirData->d_type == DT_DIR && atoi (dirData->d_name)) {

            ProgramData* proc = malloc (sizeof (ProgramData));

            proc->pid = atoi (dirData->d_name);
	    proc->pid = -1;
	    proc->usage = -999.999;
	    proc->process_name[0] = '\0';
	    proc->utime = 0;
	    proc->stime = 0;
            // get the path to the stat file of the pid dir.
            char path[300];
            snprintf (path, sizeof (path), "/proc/%s/stat", dirData->d_name);

            char process_name[30];
            unsigned long utime = 0;
            unsigned long stime = 0;
            FILE* stat = fopen (path, "r");
            if (stat == NULL) {
                free (proc);
                continue;
            }

            fscanf (stat,
                    "%*d %29s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu "
                    "%lu %*d %*d %*d %*d %*d %*d %*u %*u %*ld",
                    process_name, &utime, &stime);

            proc->utime = utime;
            proc->stime = stime;
            strcpy (proc->process_name, process_name);
	
	    fclose(stat);

            if (*size >= *capacity) {

                int new_cap = *capacity * 2;
                ProgramData** temp = malloc (new_cap * sizeof (ProgramData*));
                if (temp == NULL) {
                    fprintf (stderr, "Failed to reallocate memory.\n");
                    return NULL;
                }

                // memmove(temp,procs, new_cap*sizeof(ProgramData*));

                for (int i = 0; i < *size; ++i) {
                    if (procs[i] != NULL) {
                        temp[i] = malloc (sizeof (
                            ProgramData));

                        memcpy (temp[i], procs[i], sizeof (ProgramData));
                    }
                }

                for (int i = 0; i < *capacity; i++) {
                    free (procs[i]);
                }
		free(procs);

                procs = temp;
                *capacity = new_cap;
            }
            procs[*size] = proc;

            (*size)++;
        }
    }

    closedir (dir);
    return procs;
}

ProgramData* _procCpuData (ProgramData* data) {
	

    ProgramData** procs = malloc (10 * sizeof (ProgramData*));

    int all_procs_size = 0;
    int capacity = 10;
    ProgramData** procs2 = read_proc (procs, &all_procs_size, &capacity);
    unsigned long long total_cpu_time_start = _getTotalCpuTime ();


    usleep (600000);
    ProgramData** procs_last = malloc (10 * sizeof (ProgramData*));

    int all_procs_size_last = 0;
     capacity = 10;
    ProgramData** procs2_last = read_proc (procs_last, &all_procs_size_last, &capacity);
    unsigned long long total_cpu_time_last = _getTotalCpuTime ();


    unsigned long long total_cpu_time_end = _getTotalCpuTime ();

    unsigned long long elapsed_cpu_time =
        total_cpu_time_end - total_cpu_time_start;


    long num_cpus = sysconf (_SC_NPROCESSORS_CONF);

    for (int i = 0; i < all_procs_size_last; ++i) {

        ProgramData* start = procs2[i];

        ProgramData* starta = procs2_last[i];
	

	if(!start || !starta) continue;


                unsigned long long cpu_time =
                   ((starta->utime + starta->stime) - (start->utime +
                   start->stime));

                float usage = (cpu_time / (float)elapsed_cpu_time) * 100.0 *
                num_cpus;

            if (usage > data->usage) {

                   data->usage = usage;
                  strcpy (data->process_name, start->process_name);
           }

    }

    return data;
}

/*
 * ::::::::::::getHighestProcess()::::::::::::::
 * This function will return a ProgramData pointer with information about the
 * process using the highest amount of CPU. If there is any issue getting the
 * data, this function will return NULL. As a result, the function also
 * assumes the client code will handle any NULL returned.
 *
 */
ProgramData* getHighestProcess () {
    ProgramData* data = malloc (sizeof (ProgramData));
    data->pid = -1;
    data->usage = -999.999;
    data->process_name[0] = '\0';
    data->utime = 0;
    data->stime = 0;
    _procCpuData (data);
    return data;
}

int main (int argc, char* argv[]) {

    ProgramData* data = getHighestProcess ();

     printf ("%s\n", data->process_name);
    free (data);
    return 0;
}
