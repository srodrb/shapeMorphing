

#ifndef MESHGEN_H_
#define MESHGEN_H_

/*
 * Esta clase puede servir de clase principal a la hora de crear las mallas, 
 * la idea es que seleccione entre los distintos tipos de perfiles de la familia
 * NACA que tenemos.
 * Si el constructor recibe un string con el tipo de familia de perfil que buscamos
 * puede encargarse de seleccionar la clase que genera sus coordenadas.
 */


#include "common.h"
#include "parameterStructs.h"
#include "naca4.h"


class meshGen : public naca4 {
    /*
     * Doc: Clase que hereda, bien de una clase que implemente las b-splines, o bien
     *      de una clase que tenga generadas las coordenadas de los perfiles[]
     */
    
    public:
        meshGen (naca4parameters parameters);
        ~meshGen(){};

        void generateBlockMeshDict();

    protected:
        int a;
    
    };

#endif