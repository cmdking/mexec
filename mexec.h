/*
 * File:                
 * Author:              
 * CS-username:         
 * Date:                2022-10-08
 * mexec is a program that executes pipelines. A pipeline is a composition of programs
 * that run in parallel where a program's output becomes the next program's input.
 *
 * USAGE: ./mexec [FILE]
*/
#ifndef __mexec
#define __mexec

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

/**
  * Extracting data from 1D array and save the first word in an array.
  * Then save all other data in another 2D array. Commanss and arguments are
  * passed by reference.
  *
  *  @param: buffer - Char pointer
  *          num_of_argument - The amount of agruments
  *
  *  @return: extracted data
  */
char** extract_arguments(char *buffer,int *num_of_argument);

/**
  * Takes commands and argument and execute them using execvp system call.
  * System call takes data from pipe depending on the value of input and output.
  *
  *  @param:    in - input - integer
  *             out - output - integer
  *
  */
void run_pipe(char **arguments, int in, int out);

/**
  * Counts the number of pipe present in data provided by the user.
  *
  *  @param: buffer - buff - array
  *
  *  @return: Number of arguments
  */
int pip_counter(char buffer[]);

/**
  * Takes a commands and arguments then execute them by using system cals.
  *
  *  @param: buffer - buffer - array
  */
void separation(char buffer[]);

#endif
