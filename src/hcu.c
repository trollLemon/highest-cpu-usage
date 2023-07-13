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

#include <stdio.h>   
#include <stdlib.h> 
#include <stddef.h>
#include <dirent.h>
#include <string.h>
#define MAX_NAME_LENGTH  257









/*TODO: get % of cpu a process is using*/



/*  
 *  :::::::::::::ProgramData:::::::::::::::
 *  This struct will hold relevant program data.
 *  Pid: this is the process id.
 *  percentage: this is the percentage of the CPU the process is using. 
 *  process_name: array of chars for process names.
 *
 */
typedef 
struct 
{
    unsigned int pid;
    unsigned long perc;
    char process_name[MAX_NAME_LENGTH];
} ProgramData;



/*
 * Print help text
 */
void 
PrintHelp() 
{}




/*
 * ::::::::::::_procCpuData::::::::::::::
 *  Given a non-null ProgramData pointer, this function will assign the pid, cpu %, and process name of the process with the highest CPU usage.
 *  to the ProgramData pointer
 *  
 *  To get this data, the function shall read each PID directory in /proc, while updating the pid, cpu  and process_name in O(n) time.
 *
 *
 */
void 
_procCpuData(ProgramData *data) 
{ 
	DIR *proc = opendir("/proc");
	
	struct dirent* dirData;

	while((dirData = readdir(proc)) != NULL)
	{
	
	//filter out ./ and ../ otherwise we will go out of /proc and into some other directory, that would be weird if that happened.
	//also make sure we are looking at another directory and dont treat a text file like one
	if (dirData->d_type == DT_DIR && strcmp(dirData->d_name, "..") != 0 && strcmp(dirData->d_name, ".") != 0)
	{
		
	    //get the path to the stat file of the pid dir.
	    char path[300];
            snprintf(path, sizeof(path), "/proc/%s/stat", dirData->d_name);
            FILE* file = fopen(path, "r");
	    if (file != NULL)
	    {
	    
		    int pid = 0;
		    char process_name[MAX_NAME_LENGTH];
		    unsigned long user_time = 0;
		    unsigned long system_time = 0;
	    	    fscanf(file, "%d %s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %lu %lu",
       			&pid, process_name, &user_time, &system_time);
	    	
		    //if we found a process with more cpu, update the data pointer
		    if(user_time + system_time > data->perc){
			
			    data->pid = pid;
			    data->perc= user_time+system_time;
			    strcpy(data->process_name, process_name);

		    }
		    

	    }
	
	}

	}


}





/*
 * ::::::::::::getHighestProcess()::::::::::::::
 * This function will return a ProgramData pointer with information about the process using the highest amount of CPU.
 * If there is any issue getting the data, this function will return NULL. As a result, the function also assumes 
 * the client code will handle any NULL returned.
 *
 */
ProgramData*
getHighestProcess()
{
	ProgramData *data = malloc (sizeof(ProgramData));
	data->pid = -1;
	data->perc= 0;
	data->process_name[0] = '\0';
	_procCpuData(data);


	return data;
}


int 
main(int argc, char *argv[]) 
{ 
	
 	ProgramData *data = getHighestProcess();
	
	printf("%s", data->process_name);
	free(data);
	return 0; 



}
