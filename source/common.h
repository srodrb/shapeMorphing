/*
 * Fichero que contiene las librerias comunes empleadas por el programa CFoilMesh
 */

#ifndef  COMMON_H_
#define COMMON_H_

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <cstdlib>
#include <getopt.h>

using namespace std;

//nombre del programa

#include <stdio.h>

char *c, cad[300];

char* readPath()
{
    FILE *fp;

    fp=fopen(".path.txt", "r");

    while(c=fgets(cad,300,fp))
        //printf("%s",cad);
    
    return cad;
}


#endif 
