#include "data.h"


data::data()
{
    Ni = 100;

    xu = (double*)malloc(Ni*sizeof(double)); 
    xl = (double*)malloc(Ni*sizeof(double)); 
    zu = (double*)malloc(Ni*sizeof(double)); 
    zl = (double*)malloc(Ni*sizeof(double)); 
}
