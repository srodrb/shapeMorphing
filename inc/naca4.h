/*
 * Definicion de la clase airfoi. Esta clase se encarga de generar las coordenadas del perfil
 */

#ifndef NACA4_H_
#define NACA4_H_

#include "common.h"
#include "data.h"

#include "parameterStructs.h"


class naca4 : public data{
public:
    naca4 (float chord,float M, float P, float T, float angle);
    //necesito un constructor que funcione a partir de una estructura
    naca4 (naca4parameters parameters);
    ~naca4 () {};

    //__________Metodos de la clase____________//
    void CreateCoordinates();
    void meshGen();
    void meshGen2();
    void exportCoordinates();
    virtual void plot(); //otras clases tienen el mismo metodo, por eso es virtual
    
protected:
   float c,m,p,t,alpha,scale;
   int q;
   /*
    * Parametros de entrada:
    *   q = punto de separacion entre el borde de salida y de entrada
    */
   static const float pi = 3.1416;
};



#endif

