
#ifndef PARAMETERSTRUCTS_H_
#define PARAMETERSTRUCTS_H_

struct naca4parameters{
    /*
     * Estructura destinada a contener los parametros necesarios para definir la geometria
     * de los perfiles NACA de 4 cifras.
     * Contiene ademas parametros adicionales como el angulo de ataque o el espesor de la
     * capa limite para la malla.
     */
    float c;
    float m;
    float p;
    float t;
    float angle; //angle of attack
    float bl_thickness;//boundary layer thickness

    //contructor de la estructura
    naca4parameters(float C, 
                    float M,
                    float P,
                    float T,
                    float AOA,
                    float BL_THICKNESS
            );
};


#endif