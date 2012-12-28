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
#include "splineShape.h"
#include "parameterStructs.h"


int main(int argc, const char *argv[])
{
    printf("Iniciando el modulo principal\n");
    //creamos la clase perfil e iniciamos los metodos asociados
    float m = 7.0;
    float p = 5.0;
    float t = 12.0;
    float c = 1.0; //cuerda del perfil

    naca4parameters parameters(c,m,p,t,0.0,0.0);//esta es la estructura que trabaja con los perfiles naca 4 series.
    splineShape shape(4,parameters);            //podemos trabajar mas comodamente de esta forma.
    
    shape.meshGen();//los metodos tambien funcionan desde la herencia de este modo
    shape.calcSplines();
    shape.coordinates();
    shape.exportCoordinates("airfoilCoordinates.dat");

    shape.plot();
/*
    displacementStruct displacement(315.0,0.08);
    int pointID = 9;

    shape.modifyControlPoint(pointID, displacement);
    shape.plot();*/

    return 0;
}




