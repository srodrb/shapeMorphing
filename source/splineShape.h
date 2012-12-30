
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

displacementStruct::displacementStruct(double angle, double distance)
{
    //simple constructor
    theta = angle*3.14159265/180.0;
    norm  = distance;
}



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

splineShape::splineShape(int controlPoints, naca4parameters parameters):naca4 (parameters)
{
    /* La clase tiene dos etapas:
     *      - Seleccionar las coordenadas del perfil que van a ser los puntos de control
     *      - Crear los splines a partir de esos puntos de control.
     */
    
    n = 8;                     //number of control points. Source adds +1
    t = 3;                     //degree of polynomial = t-1
    tnp = 15;
    pts = new point[2*n -1];      //double -1  cause contains upper and lower surface. One point is duplicated.

    //Iniciamos el constructor con la spline basica sobre la que luego trabajaremos
    initialSpline();
}

void splineShape::initialSpline()
{
      int *u;
      int i;
     
     //____________________________________Creamos el spline para la superficie superior
      
      /*
       * Esta parte se puede emplear para hacer una distribucion senoidal de los puntos
       
      static double step;
      double x;
      int index;
      step = pi/n;
       for (int i = 0; i < n; i++) {//este debe ser n+1
           x = i*step;
           index = (int) ceil( Ni*(0.5*(1-cos(x))) );
            
           if(index == Ni) { index = Ni-1; }
           pts[i].x = xl[index]; pts[i].y=zl[index]; pts[i].z=0.0;
       }


      //_______________________________Generamos los puntos para el spline de la sup. inferior.
      for (int i = 0; i < n; i++) {
          x = i*step;
          index = (int) ceil( Ni*(0.5*(1-cos(x))) );
            
          index = Ni-index;
          if(index == Ni) { index = Ni-1;} //non-"index out of bounds" condition

          //Una vez tenemos calculados los indices creamos los puntos automaticamente
          pts[i+n].x = xu[index];     pts[i+n].y=zu[index];     pts[i+n].z=0.0;
      }
      //_____________________________________________________________________________
    */
      

      //Debido a la distribucion senoidal de los puntos, no se situan donde tenia pensado,
      //tengo que ver como los ajusto...
        pts[0].x = xu[Ni-1];     pts[0].y=zu[Ni-1];    pts[0].z=0.0;
        pts[1].x = xu[84];       pts[1].y=zu[84];      pts[1].z=0.0;
        pts[2].x = xu[70];       pts[2].y=zu[70];      pts[2].z=0.0;
        pts[3].x = xu[49];       pts[3].y=zu[49];      pts[3].z=0.0;
        pts[4].x = xu[30];       pts[4].y=zu[30];      pts[4].z=0.0;
        pts[5].x = xu[16];       pts[5].y=zu[16];      pts[5].z=0.0;
        pts[6].x = xu[3];        pts[6].y=zu[3];       pts[6].z=0.0;
        pts[7].x = xu[0];        pts[7].y=zu[0];       pts[7].z=0.0;
        
        pts[8].x =  xl[3];       pts[8].y=zl[3];       pts[8].z=0.0;
        pts[9].x =  xl[15];      pts[9].y=zl[15];      pts[9].z=0.0;
        pts[10].x = xl[29];      pts[10].y=zl[29];     pts[10].z=0.0;
        pts[11].x = xl[49];      pts[11].y=zl[49];     pts[11].z=0.0;
        pts[12].x = xl[70];      pts[12].y=zl[70];     pts[12].z=0.0;
        pts[13].x = xl[84];      pts[13].y=zl[84];     pts[13].z=0.0;
        pts[14].x = xu[Ni-1];    pts[14].y=zu[Ni-1];   pts[14].z=0.0;


      out_pts = new point[2*Ni]; //antes teniamos Ni elementos en upper y Ni en lower, ahora debemos tener 2 Ni
      bspline(tnp-1, t, pts, out_pts);
      exportControlPoints();
      updateControlPoints();
}

void splineShape::calcSplines()
{
    /*
     * Este metodo tiene que trabajar sobre los puntos de control que ya existen.
     * Su finalidad es servir como un "update" al calculo de la spline cuando
     * hacemos alguna variacion sobre los puntos de control ya calculados
     */

       int *u;
       bspline(14, t, pts, out_pts);//antes era tpn-1

      //una vez tengo la salida de los puntos del spline quiero sacar los puntos
      //de control marcados para la curva para ver como se comporta a diferentes 
      //grados del polinomio.

      exportControlPoints();
      exportSplines();
      updateControlPoints();
}


void splineShape::modifyControlPoint(int pointID, displacementStruct displacement)
{
    /*
     * Este metodo pretende implementar un movimiento basico de un punto de control:
     * Argumentos:
     *      - pointID: <unsigned int> identificador del punto sobre el que vamos a operar
     *      - displacement: 
     */

    
    double xdisplacement,ydisplacement;
    
    //Criterio, los puntos de control del borde de ataque y del borde de salida quedan fijos.
    if ( pointID == 0 or pointID == n+1 or pointID == 2*n)//TODO anadir la condicion de pointID == 0
    {
        printf("Fixed point, implement an Error sentence!\n");
        xdisplacement = 0.0;
        ydisplacement = 0.0;
    }
    
    else
    {
        xdisplacement = displacement.norm * cos(displacement.theta);
        ydisplacement = displacement.norm * sin(displacement.theta);
    }
    pts[pointID].x += xdisplacement;
    pts[pointID].y += ydisplacement;
    
   calcSplines();
   updateControlPoints();
}

void splineShape::updateControlPoints()
{
    /*
     * El metodo actualiza los puntos de control para poder efectuar modificaciones
     * sobre ellos en la siguiente iteracion.
     *
     * Tengo que calcular los puntos en base a la cuerda porque se desplazan en 
     * sucesivas iteraciones... vamos con ello.
     *
     * Ayudaria tambien hacer una distribucion lineal de los puntos, pero eso solo 
     * importa para la primera aproximacion y esa funciona bastante bien, asi que
     * no es tan importante
     */


        pts[0].x = out_pts[0].x;                             pts[0].y=out_pts[0].y;                            pts[0].z=0.0;
        pts[1].x = out_pts[calcIndex(0.95,"upper")].x;       pts[1].y=out_pts[calcIndex(0.95,"upper")].y;      pts[1].z=0.0;
        pts[2].x = out_pts[calcIndex(0.80,"upper")].x;       pts[2].y=out_pts[calcIndex(0.80,"upper")].y;      pts[2].z=0.0;
        pts[3].x = out_pts[calcIndex(0.50,"upper")].x;       pts[3].y=out_pts[calcIndex(0.50,"upper")].y;      pts[3].z=0.0;
        pts[4].x = out_pts[calcIndex(0.20,"upper")].x;       pts[4].y=out_pts[calcIndex(0.20,"upper")].y;      pts[4].z=0.0;
        pts[5].x = out_pts[calcIndex(0.05,"upper")].x;       pts[5].y=out_pts[calcIndex(0.05,"upper")].y;      pts[5].z=0.0;
        pts[6].x = out_pts[calcIndex(0.005,"upper")].x;      pts[6].y=out_pts[calcIndex(0.005,"upper")].y;     pts[6].z=0.0;

        pts[7].x = 0.0;                                      pts[7].y=0.0;                                     pts[7].z=0.0;
        
        pts[8].x =  out_pts[calcIndex(0.005,"lower")].x;     pts[8 ].y=out_pts[calcIndex(0.005,"lower")].y;    pts[8].z=0.0;
        pts[9].x =  out_pts[calcIndex(0.05,"lower")].x;      pts[9 ].y=out_pts[calcIndex(0.05,"lower")].y;     pts[9].z=0.0;
        pts[10].x = out_pts[calcIndex(0.20,"lower")].x;      pts[10].y=out_pts[calcIndex(0.20,"lower")].y;     pts[10].z=0.0;
        pts[11].x = out_pts[calcIndex(0.50,"lower")].x;      pts[11].y=out_pts[calcIndex(0.50,"lower")].y;     pts[11].z=0.0;
        pts[12].x = out_pts[calcIndex(0.80,"lower")].x;      pts[12].y=out_pts[calcIndex(0.80,"lower")].y;     pts[12].z=0.0;
        pts[13].x = out_pts[calcIndex(0.95,"lower")].x;      pts[13].y=out_pts[calcIndex(0.95,"lower")].y;     pts[13].z=0.0;

        //ahora actualizamos los valores de los vectores xu,zu,xl,zl..
        for (int i = 0; i < Ni; i++) {
            //printf("xu[%d] = %f\n", i,out_pts[Ni-i].x);
            xu[i] = out_pts[Ni-i].x;
            zu[i] = out_pts[Ni-i].y;
        }
        xu[0] = 0.0; zu[0] = 0.0;
        for (int i = 0; i < Ni; i++) {
            //printf("xl[%d] = %f\n", i,out_pts[Ni+i].x);
            xl[i] = out_pts[Ni+i].x;
            zl[i] = out_pts[Ni+i].y;
        }
}


void splineShape::exportControlPoints()
{
    FILE *out;
    out = fopen("controlPoints.dat", "w");
    for (int i = 0; i < tnp; i++) {
          fprintf(out, "%f\t%f\t%d\n", pts[i].x,pts[i].y,i); 
    }
    fclose(out);
    printf("Exported control points to controlPoints.dat.\n");
}

void splineShape::exportSplines()
{
    FILE *out;
    out = fopen("airfoilCoordinates.dat", "w");
    for (int i = 0; i < 2*Ni-1; i++) {
          fprintf(out, "%f\t%f\t%d\n", out_pts[i].x,out_pts[i].y,i); 
    }
    fclose(out);
    printf("Exported spline points to airfoilCoordinates.dat.\n");
}

void splineShape::plot()
{
    int output = system ("gnuplot -persist plotSpline.gp");
}


//================= METODOS PARA EL CALCULO DE LAS BSPLINES ======================//

void splineShape::bspline(int n, int t, point *control, point *output)
{

        /*********************************************************************

        Parameters:
          n          - the number of control points minus 1
          t          - the degree of the polynomial plus 1
          control    - control point array made up of point stucture
          output     - array in which the calculate spline points are to be put
          num_output - how many points on the spline are to be calculated

        Pre-conditions:
          n+2>t  (no curve results if n+2<=t)
          control array contains the number of points specified by n
          output array is the proper size to hold num_output point structures


        **********************************************************************/
      int *u;                         //puntero que almacena posteriormente en un vector los puntos
      double increment,interval;
      point calcxyz;                    //estructura punto, xyz
      int output_index;

      u=new int[n+t+1];
      compute_intervals(u, n, t);

      increment=(double) (n-t+2)/(2*Ni-1);  // how much parameter goes up each time
      interval=0;

      for (output_index=0; output_index<2*Ni-1; output_index++)
      {
        compute_point(u, n, t, interval, control, &calcxyz);
        output[output_index].x = calcxyz.x;
        output[output_index].y = calcxyz.y;
        output[output_index].z = calcxyz.z;
        interval=interval+increment;  // increment our parameter
      }
      output[2*Ni-1].x=control[n].x;   // put in the last point
      output[2*Ni-1].y=control[n].y;
      output[2*Ni-1].z=control[n].z;

      //ahora que tenemos los calculos hechos quiero generar un fichero de salida
      //para Gnuplot para poder ver los resultados.
      FILE *out;
      out = fopen("splinePoints.dat","w");
      for (int i = 0; i < 2*Ni; i++) {
          fprintf(out, "%f\t%f\t%f\n", output[i].x,output[i].y,output[i].z);
          printf("Coordenadas: %f\t%f\t%d\n", output[i].x,output[i].y,i,n);
      }
      fclose(out);

      delete u;
}

double splineShape::blend(int k, int t, int *u, double v)  // calculate the blending value
{
  double value;

  if (t==1)         // base case for the recursion
  {
    if ((u[k]<=v) && (v<u[k+1]))
      value=1;
    else
      value=0;
  }
  else
  {
    if ((u[k+t-1]==u[k]) && (u[k+t]==u[k+1]))  // check for divide by zero
      value = 0;
    else
    if (u[k+t-1]==u[k]) // if a term's denominator is zero,use just the other
      value = (u[k+t] - v) / (u[k+t] - u[k+1]) * blend(k+1, t-1, u, v);
    else
    if (u[k+t]==u[k+1])
      value = (v - u[k]) / (u[k+t-1] - u[k]) * blend(k, t-1, u, v);
    else
      value = (v - u[k]) / (u[k+t-1] - u[k]) * blend(k, t-1, u, v) +
          (u[k+t] - v) / (u[k+t] - u[k+1]) * blend(k+1, t-1, u, v);
  }
  return value;
}

void splineShape::compute_intervals(int *u, int n, int t)   // figure out the knots
{
  int j;

  for (j=0; j<=n+t; j++)
  {
    if (j<t)
      u[j]=0;
    else
    if ((t<=j) && (j<=n))
      u[j]=j-t+1;
    else
    if (j>n)
      u[j]=n-t+2;  // if n-t=-2 then we're screwed, everything goes to 0
  }
}

void splineShape::compute_point(int *u, int n, int t, double v, point *control,point *output)
{
  int k;
  double temp;

  // initialize the variables that will hold our outputted point
output->x=0;
  output->y=0;
  output->z=0;

  for (k=0; k<=n; k++)
  {
    temp = blend(k,t,u,v);  // same blend is used for each dimension coordinate
    output->x = output->x + (control[k]).x * temp;
    output->y = output->y + (control[k]).y * temp;
    output->z = output->z + (control[k]).z * temp;
  }
}

int splineShape::calcIndex(double value, const char* zone)
{
    /*
     * Si se tienen dudas sobre su funcionamiento es de gran ayuda
     * imprimir por pantalla los valores de las coordenadas de las 
     * splines para ver su ordenamiento.
     */
    if(zone == "upper")
    {
        for (int i = 0; i < 2*Ni-1; i++) {
            if (out_pts[i].x >= value and out_pts[i+1].x < value)
            {
                return i;
                break;
            }
        }
    }
    else
    {
        for (int i = 0; i < 2*Ni-1; i++) {
            if(out_pts[i].x <= value and out_pts[i+1].x > value)
            {
                return i+1; //sumamos uno porque vamos con retraso...
                break;
            }
        }
    }
}
#endif
