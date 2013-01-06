/*
 * Fichero que contiene las librerias comunes empleadas por el programa CFoilMesh
 */

#ifndef  COMMON_H_
#define COMMON_H_

#include <stdio.h>
#include <unistd.h> //for getcwd (char* getcwd(char* buf, size_t size))
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <cstdlib>
#include <getopt.h>

using namespace std;

//nombre del programa

#include <stdio.h>


char* readPath();
char* setPath(const char* route,const char* filename);

#endif 
