/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  Fichero principal del proyecto CMeshFoil.       
 *
 *        Version:  1.0
 *        Created:  26/12/12 08:39:00
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Samuel Rodriguez Bernabeu (srodrb), srodrb@gmail.com
 *   Organization:  University of Leon (Spain)
 *
 * =====================================================================================
 */

#include "common.h"
#include "data.h"

#include "meshGen.h"

#include "splineShape.h"
#include "parameterStructs.h"
#include "naca4.h"


int main(int argc, const char *argv[])
{
    printf("Iniciando el modulo principal\n");
    //_______________________________________Airfoil parameters
    float m = 0.0;
    float p = 0.0;
    float t = 12.0;
    float c = 1.0; 

    naca4parameters parameters(c,m,p,t,0.0,0.0);
    //meshGen naca7512 (parameters);
    splineShape shape(4,parameters);            
    shape.plot();
    /*
    splineShape shape(4,parameters);            

    
    
    shape.calcSplines();
    shape.coordinates();
    shape.exportCoordinates("airfoilCoordinates.dat");

    shape.plot();
    displacementStruct displacement(315.0,0.05);
    int pointID = 11;
    shape.modifyControlPoint(pointID, displacement);
    shape.plot();

    //ahora vamos a crear directamente la malla a partir de la clase derivada
    meshGen naca0012 (parameters);
    */

    return 0;
}




