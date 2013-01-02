#include "parameterStructs.h"

naca4parameters::naca4parameters(float C, float M, float P, float T, float AOA, float BL_THICKNESS)
{
    
    c = C;
    m = M;
    p = P;
    t = T;
    angle = AOA;
    bl_thickness = BL_THICKNESS;
}