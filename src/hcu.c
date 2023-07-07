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


/*  
 *  :::::::::::::ProgramData:::::::::::::::
 *  This struct will hold relevant program data.
 *  Pid: this is the process id.
 *  percentage: this is the percentage of the CPU the process is using. 
 *  process_name: flexible array of chars so we can account for variable-sized process names.
 *
 */
typedef 
struct 
{
    unsigned int pid;
    char percentage[4];
    char process_name[];
} ProgramData;



/*
 * Print help text
 */
void 
PrintHelp() 
{}


/*
 * ::::::::::::getHighestProcess()::::::::::::::
 * This function will return a ProgramData pointer with information about the process using the highest amount of CPU.
 * If there is any issue getting the data, this function will return NULL. As a result, the function also assumes 
 * the client code will handle any NULL returned.
 */
ProgramData *getHighestProcess()
{
return NULL;
}


int 
main(int argc, char *argv[]) 
{ return 0; }
