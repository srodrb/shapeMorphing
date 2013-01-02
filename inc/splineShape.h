
#ifndef SPLINESHAPE_H_
#define SPLINESHAPE_H_

//clase que hereda de naca4 (por el momento) y que crea splines a partir de las coordenadas

#include "naca4.h"
/*********************************************************************

 Simple b-spline curve algorithm

 Copyright 1994 by Keith Vertanen (vertankd@cda.mrs.umn.edu)

 Released to the public domain (your mileage may vary)

**********************************************************************/


#include <stdlib.h>
#include "common.h"


struct point {
  double x;
  double y;
  double z;
};

struct displacementStruct{
    double theta; //angulo de desplazamiento
    double norm;  //modulo del vector.

    displacementStruct(double angle, double distance);
};




class splineShape : public virtual naca4 {
    /*
     * Doc: Not described.
     *
     *  @controlPoints: unsigned int.
     *                  Numero de puntos de control empleados para crear el b-spline.
     *                  Puntos sobre upper o lower surface.
     *                  El borde de ataque y el de salida no cuentan para el computo.
     */
    
    public:
        splineShape (int controlPoints, naca4parameters parameters);
        ~splineShape(){};

        //-----Metodos de la clase------//
        void initialSpline();
        void calcSplines();
        void modifyControlPoint(int pointID, displacementStruct displacement);
        void exportControlPoints();
        void exportSplines();

        virtual void plot();//declarado como virtual para que el resto de clases tengan que implementarlo

    protected:
        point *pts;          // puntero de estructuras punto con el que funcionan las funciones.
        int n,t,tnp;
        point * out_pts;     //puntero que utilizan las funciones para el calculo de las splines
        /*
         * n; numero de puntos de control
         * t: grado de la b-spline
         */

     private:
        void bspline(int n, int t, point *control, point *output);
        double blend(int k, int t, int *u, double v);
        void compute_intervals(int *u, int n, int t);
        void compute_point(int *u, int n, int t, double v, point *control,point *output);
        void updateControlPoints();
        int calcIndex(double value, const char* zone);
    };
    
#endif
