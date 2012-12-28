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

int menu()
{
	static int option = 0;
	printf ("           Choose an option: \n");
	printf (
           "	  0			NACA 4 digits series.\n"
           "	  1			NACA 5 digits series.\n"
           "	  2			NACA 6 digits series.\n");

	cout << "	Selection?: "; cin >> option;
	return option;
}

#endif 
