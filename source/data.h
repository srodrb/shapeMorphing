#ifndef DATA_H_
#define DATA_H_

#include "common.h"


class data {
    /*
     * Doc: Clase base del resto
     */
    
    public:
        data ();
        ~data(){};


    protected:
        int Ni;                      //numero de puntos del perfil
        double *xu,*zu,*xl,*zl;      //punteros para almacenar las coordenadas
    };

data::data()
{
    Ni = 100;

    xu = (double*)malloc(Ni*sizeof(double)); 
    xl = (double*)malloc(Ni*sizeof(double)); 
    zu = (double*)malloc(Ni*sizeof(double)); 
    zl = (double*)malloc(Ni*sizeof(double)); 
}


#endif
