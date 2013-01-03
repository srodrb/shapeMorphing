#include "naca4.h"



naca4::naca4(float chord,float M, float P, float T, float angle) : data()
{
    m       = 0.01*M;
    p       = 0.10*P;
    t       = 0.01*T;
    alpha   = (angle*pi)/180.0;
    c       = chord;

    CreateCoordinates();
    exportCoordinates();
}

naca4::naca4 (naca4parameters parameters) : data()
{
    m       = 0.01* parameters.m;
    p       =  0.1* parameters.p;
    t       = 0.01* parameters.t;
    c       = parameters.c;
    alpha   = (parameters.angle*pi)/180.0;

    printf("Airfoil info: m = %f, p = %f, t = %f, c=%f\n", m,p,t,c);
    CreateCoordinates();
    exportCoordinates();
}

void naca4::CreateCoordinates()
{

    //calculate x as a linspace distribution
    double x[Ni];
    double step = pi/(Ni-1);
    for (int i = 0; i < Ni; i++) {
        x[i] = i*step;
        //now apply senoidal distribution to achive more detail con trailing and leading edges.
        x[i] = c*(0.5*(1-cos(x[i])));
        //debug_printf("x[%d] = %.6f\n", i+1,x[i]);
    }

    //y_t == z_t. Calculate thickness distribution
    double z_t[Ni];
    double f0,f1,f2,f3,f4,f5;
    //debug_printf("-----Showing camber line-----\n");
    for (int i = 0; i < Ni; i++) {
        f0 = (t*c)/0.2;
        f1 =  0.296900*pow(x[i]/c,0.5);
        f2 = -0.126000*(x[i]/c);
        f3 = -0.351600*pow(x[i]/c,2);
        f4 =  0.284300*pow(x[i]/c,3);
        f5 = -0.103600*pow(x[i]/c,4);
        
        z_t[i] = f0*(f1+f2+f3+f4+f5);
        //debug_printf("z_t[%d] = %.6f\n", i+1,z_t[i]);
    }

    if(p == 0 or m == 0){
        //debug_printf("-----Symmetric NACA 4 digits naca4-----\n");
        for (int i = 0; i < Ni; i++) {
            xu[i] =     x[i];
            xl[i] =     x[i];
            zu[i] =     z_t[i];
            zl[i] =    -z_t[i];
        }
    }


    else{
        //debug_printf("-----Non symmetric NACA 4 digits naca4-----\n");
        double z_c[Ni];
        double theta[Ni];
        for (int i = 0; i < Ni; i++) {
            if(x[i] <= p*c)
            {
                z_c[i] = ((m*x[i])/pow(p,2))*(2*p -x[i]/c);
                theta[i] = atan( (m/pow(p,2)) * (2*p - 2*x[i]/c) );
            }
            else
            {
                z_c[i] =  (m*(c-x[i])/pow((1-p),2))*(1 +x[i]/c -2*p);
                theta[i] = atan( (m/pow(1-p,2)) * (-2*x[i]/c + 2*p) );
            }        
            //calculate upper and lower coordinates in the same loop
            xu[i] = x[i] -z_t[i]*sin(theta[i])   ;
            xl[i] = x[i] +z_t[i]*sin(theta[i])   ;
            zu[i] = z_c[i] +z_t[i]*cos(theta[i]) ;
            zl[i] = z_c[i] -z_t[i]*cos(theta[i]) ;
            }
    }

}

void naca4::meshGen()
{
    //_____Virtual wind tunnel dimensions_______//
    const float H = 8.0;
    const float W = 0.5;
    const float D = 16.0;
    //____Mesh density control parameters______//
    const int Nx = 150;
    const int ND = 30;
    const int NT = 200;
    const int NW = 1;
    //____Mesh expansion ratios________________//
    const float ExpT = 5;
    const float ExpD = 15;
    const float ExpA = 5;
    scale = 1.0; 
    //:::::::::::::::::::::::::::::::::::::::://
    int Nleading = 40;        //divisiones hacia el borde de entrada
    int Ntrailing = 70;       //divisiones hacia el borde de salida
    q = 35;                   //cuanto menor sea mas cerca estoy del borde de entrada (leading)

    float NoseX =  (-H+xu[q])*cos(alpha); //cout << "NoseX value: " << NoseX << endl;
    float NoseZ = -(-H+xu[q])*sin(alpha); //cout << "NoseZ value: " << NoseZ << endl;

    //calculamos los vertices que definen la geometria de la malla
    float vertices[24][3];

    vertices[0][0] = NoseX;     vertices[0][1] = W;     vertices[0][2] = NoseZ;
    vertices[1][0] = xu[q];     vertices[1][1] = W;     vertices[1][2] = H;
    vertices[2][0] = xu[Ni-1];  vertices[2][1] = W;     vertices[2][2] = H;
    vertices[3][0] = D;         vertices[3][1] = W;     vertices[3][2] = H;
    vertices[4][0] = 0.0;       vertices[4][1] = W;     vertices[4][2] = 0.0;
    vertices[5][0] = xu[q];     vertices[5][1] = W;     vertices[5][2] = zu[q];//error
    vertices[6][0] = xl[q];     vertices[6][1] = W;     vertices[6][2] = zl[q];//error
    vertices[7][0] = xu[Ni-1];  vertices[7][1] = W;     vertices[7][2] = zu[Ni-1];
    vertices[8][0] = D;         vertices[8][1] = W;     vertices[8][2] = zu[Ni-1];
    vertices[9][0] = xl[q];     vertices[9][1] = W;     vertices[9][2] = -H;
    vertices[10][0] = xu[Ni-1]; vertices[10][1] = W;    vertices[10][2] = -H;
    vertices[11][0] = D;        vertices[11][1] = W;    vertices[11][2] = -H;

    vertices[12][0] = NoseX;        vertices[12][1] = -W;       vertices[12][2] = NoseZ;
    vertices[13][0] = xu[q];        vertices[13][1] = -W;       vertices[13][2] = H;
    vertices[14][0] = xu[Ni-1];     vertices[14][1] = -W;       vertices[14][2] = H;
    vertices[15][0] = D;            vertices[15][1] = -W;       vertices[15][2] = H;
    vertices[16][0] = 0.0;          vertices[16][1] = -W;       vertices[16][2] = 0.0;
    vertices[17][0] = xu[q];        vertices[17][1] = -W;       vertices[17][2] = zu[q];//error
    vertices[18][0] = xl[q];        vertices[18][1] = -W;       vertices[18][2] = zl[q];//error
    vertices[19][0] = xu[Ni-1];     vertices[19][1] = -W;       vertices[19][2] = zu[Ni-1];
    vertices[20][0] = D;            vertices[20][1] = -W;       vertices[20][2] = zu[Ni-1];
    vertices[21][0] = xl[q];        vertices[21][1] = -W;       vertices[21][2] = -H;
    vertices[22][0] = xu[Ni-1];     vertices[22][1] = -W;       vertices[22][2] = -H;
    vertices[23][0] = D;            vertices[23][1] = -W;       vertices[23][2] = -H;

    //Generamos el fichero blockMeshDict
    FILE *out;
    //int output = system("rm -r /home/samuel/workSpace/shapeMorphing/OFcase/constant/polyMesh/*");//para hacer las cosas un poco mas rapido
    //out = fopen("/home/samuel/workSpace/shapeMorphing/OFcase/constant/polyMesh/blockMeshDict","w");//creo el fichero en mi ruta
    out = fopen("blockMeshDict","w");

    fprintf(out, "/*--------------------------------*- C++ -*----------------------------------*\\ \n");
    fprintf(out, "| =========                 |                                                 | \n");
    fprintf(out, "| \\\\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox           | \n");
    fprintf(out, "|  \\\\    /   O peration     | Version:  2.1.0                                 | \n");
    fprintf(out, "|   \\\\  /    A nd           | Web:      www.OpenFOAM.com                      | \n");
    fprintf(out, "|    \\\\/     M anipulation  |                                                 | \n");
    fprintf(out, "\\*---------------------------------------------------------------------------*/ \n");
    fprintf(out, "FoamFile                                                                        \n");
    fprintf(out, "{                                                                               \n");
    fprintf(out, "    version     2.0;                                                            \n");
    fprintf(out, "    format      ascii;                                                          \n");
    fprintf(out, "    class       dictionary;                                                     \n");
    fprintf(out, "    object      blockMeshDict;                                                  \n");
    fprintf(out, "}                                                                               \n");
    fprintf(out, "// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * // \n");
    fprintf(out, "\n");


    fprintf(out, "convertToMeters %f; \n", scale);
    fprintf(out, "\n");
    fprintf(out, "vertices \n");       
    fprintf(out, "( \n");
    for (int i = 0; i < 24; ++i)
    {
        fprintf(out, "      (%f %f %f)\n", vertices[i][0],vertices[i][1],vertices[i][2]);
    }
    fprintf(out, "); \n");
    fprintf(out, "\n");
    fprintf(out, "blocks \n");
    fprintf(out, "( \n");
    fprintf(out, "    hex (4 5 1 0 16 17 13 12)     (%d %d %d) edgeGrading (1 %f %f 1 %f %f %f %f 1 1 1 1) \n", Nleading, NT, NW, 1.0/ExpA, 1.0/ExpA, ExpT, ExpT, ExpT, ExpT);
    fprintf(out, "    hex (5 7 2 1 17 19 14 13)     (%d %d %d) simpleGrading (1 %f 1) \n", Ntrailing, NT, NW, ExpT);
    fprintf(out, "    hex (7 8 3 2 19 20 15 14)     (%d %d %d) simpleGrading (%f %f 1) \n", ND, NT, NW, ExpD, ExpT);
    fprintf(out, "    hex (16 18 21 12 4 6 9 0)     (%d %d %d) edgeGrading (1 %f %f 1 %f %f %f %f 1 1 1 1) \n", Nleading, NT, NW, 1.0/ExpA, 1.0/ExpA, ExpT, ExpT, ExpT, ExpT);
    fprintf(out, "    hex (18 19 22 21 6 7 10 9)    (%d %d %d) simpleGrading (1 %f 1) \n", Ntrailing, NT, NW, ExpT);
    fprintf(out, "    hex (19 20 23 22 7 8 11 10)   (%d %d %d) simpleGrading (%f %f 1) \n", ND, NT, NW, ExpD, ExpT);

    fprintf(out, "); \n");
    fprintf(out, "\n");
    
    fprintf(out, "edges \n");
    fprintf(out, "( \n");
    fprintf(out, "    spline 4 5 \n");
    fprintf(out, "        ( \n");
    for (int i = 1; i < q; ++i)
    {
        fprintf(out, "      (%f %f %f)\n", xu[i],W,zu[i]);//punto 1
    } 
    fprintf(out, "       ) \n");

    fprintf(out, "    spline 5 7 \n");
    fprintf(out, "        ( \n");
    for (int i = q+1; i < Ni-1; ++i)
    {
        fprintf(out , "     (%f %f %f)\n", xu[i],W,zu[i] );//punto 2

    }
    fprintf(out, "        ) \n");

    fprintf(out, "    spline 4 6 \n");
    fprintf(out, "        ( \n");
    for (int i = 1; i < q; ++i)
    {
        fprintf(out, "      (%f %f %f)\n", xl[i],W,zl[i] );//punto 3
    }
    fprintf(out, "        ) \n");

    fprintf(out, "    spline 6 7 \n");
    fprintf(out, "        ( \n");
    for (int i = q+1; i < Ni-1; ++i)
    {
        fprintf(out, "      (%f %f %f)\n", xl[i],W,zl[i] );//punto 4
    }
    fprintf(out, "        ) \n");

    fprintf(out, "    spline 16 17 \n");
    fprintf(out, "        ( \n");
    for (int i = 1; i < q; ++i)
    {
        fprintf(out, "      (%f %f %f)\n", xu[i],-W,zu[i]);//punto 5
    } 
    fprintf(out, "        ) \n");

    fprintf(out, "    spline 17 19 \n");
    fprintf(out, "        ( \n");
    for (int i = q+1; i < Ni-1; ++i)
    {
        fprintf(out , "     (%f %f %f)\n", xu[i],-W,zu[i] );//punto 6

    }
    fprintf(out, "        ) \n");

    fprintf(out, "    spline 16 18 \n");
    fprintf(out, "        ( \n");
    for (int i = 1; i < q; ++i)
    {
        fprintf(out, "      (%f %f %f)\n", xl[i],-W,zl[i] );//punto 7
    }
    fprintf(out, "        ) \n");

    fprintf(out, "    spline 18 19 \n");
    fprintf(out, "        ( \n");
    for (int i = q+1; i < Ni-1; ++i)
    {
        fprintf(out, "      (%f %f %f)\n", xl[i],-W,zl[i] );//punto 8
    }
    fprintf(out, "        ) \n");

    fprintf(out, "    arc 0 1 (%f %f %f) \n", -H*cos(pi/4)+xu[q],W,H*sin(pi/4));
    fprintf(out, "    arc 0 9 (%f %f %f) \n", -H*cos(pi/4)+xu[q],W,-H*sin(pi/4));
    fprintf(out, "    arc 12 13 (%f %f %f) \n", -H*cos(pi/4)+xu[q],-W,H*sin(pi/4));
    fprintf(out, "    arc 12 21 (%f %f %f) \n", -H*cos(pi/4)+xu[q],-W,-H*sin(pi/4));

    fprintf(out, "); \n");
    fprintf(out, "\n");
    fprintf(out, "boundary \n");
    fprintf(out, "( \n");

    fprintf(out, "    inlet \n");
    fprintf(out, "    { \n");
    fprintf(out, "        type patch; \n");
    fprintf(out, "        faces \n");
    fprintf(out, "        ( \n");
    fprintf(out, "            (1 0 12 13) \n");
    fprintf(out, "            (0 9 21 12) \n");
    fprintf(out, "        ); \n");
    fprintf(out, "    } \n");
    fprintf(out, "\n");

    fprintf(out, "    outlet \n");
    fprintf(out, "    { \n");
    fprintf(out, "        type patch; \n");
    fprintf(out, "        faces \n");
    fprintf(out, "        ( \n");
    fprintf(out, "            (11 8 20 23) \n");
    fprintf(out, "            (8 3 15 20) \n");
    fprintf(out, "        ); \n");
    fprintf(out, "    } \n");
    fprintf(out, "\n");

    fprintf(out, "    topAndBottom \n");
    fprintf(out, "    { \n");
    fprintf(out, "        type patch; \n");
    fprintf(out, "        faces \n");
    fprintf(out, "        ( \n");
    fprintf(out, "            (3 2 14 15) \n");
    fprintf(out, "            (2 1 13 14) \n");
    fprintf(out, "            (9 10 22 21) \n");
    fprintf(out, "            (10 11 23 22) \n");
    fprintf(out, "        ); \n");
    fprintf(out, "    } \n");
    fprintf(out, "\n");

    fprintf(out, "    naca4 \n");
    fprintf(out, "    { \n");
    fprintf(out, "        type wall; \n");
    fprintf(out, "        faces \n");
    fprintf(out, "        ( \n");
    fprintf(out, "            (5 4 16 17) \n");
    fprintf(out, "            (7 5 17 19) \n");
    fprintf(out, "            (4 6 18 16) \n");
    fprintf(out, "            (6 7 19 18) \n");
    fprintf(out, "        ); \n");
    fprintf(out, "    } \n");
    fprintf(out, "); \n");
    fprintf(out, " \n");
    fprintf(out, "mergePatchPairs \n");
    fprintf(out, "( \n");
    fprintf(out, "); \n");
    fprintf(out, " \n");
    fprintf(out, "// ************************************************************************* // \n");

    fclose(out);

}

void naca4::meshGen2()
{
    //_____Virtual wind tunnel dimensions_______//
    const float H = 8.0;
    const float W = 0.5;
    const float D = 16.0;
    //____Mesh density control parameters______//
    const int Nx = 250;
    const int ND = 150;
    const int NT = 300;
    const int NW = 1;
    //____Mesh expansion ratios________________//
    const float ExpT = 50;
    const float ExpD = 10;
    const float ExpA = 5;
    scale = 1.0; 
    //:::::::::::::::::::::::::::::::::::::::://
    

    float NoseX =  (-H+xu[q])*cos(alpha); //cout << "NoseX value: " << NoseX << endl;
    float NoseZ = -(-H+xu[q])*sin(alpha); //cout << "NoseZ value: " << NoseZ << endl;
    
    int Nleading  = ceil(0.3*Nx);
    int Ntrailing = Nx-Nleading;

    //calculamos los vertices que definen la geometria de la malla
    float vertices[24][3];

    vertices[0][0] = NoseX;     vertices[0][1] = W;     vertices[0][2] = NoseZ;
    vertices[1][0] = xu[q];     vertices[1][1] = W;     vertices[1][2] = H;
    vertices[2][0] = xu[Ni-1];  vertices[2][1] = W;     vertices[2][2] = H;
    vertices[3][0] = D;         vertices[3][1] = W;     vertices[3][2] = H;
    vertices[4][0] = 0.0;       vertices[4][1] = W;     vertices[4][2] = 0.0;
    vertices[5][0] = xu[q];     vertices[5][1] = W;     vertices[5][2] = zu[q];//error
    vertices[6][0] = xl[q];     vertices[6][1] = W;     vertices[6][2] = zl[q];//error
    vertices[7][0] = xu[Ni-1];  vertices[7][1] = W;     vertices[7][2] = zu[Ni-1];
    vertices[8][0] = D;         vertices[8][1] = W;     vertices[8][2] = zu[Ni-1];
    vertices[9][0] = xl[q];     vertices[9][1] = W;     vertices[9][2] = -H;
    vertices[10][0] = xu[Ni-1]; vertices[10][1] = W;    vertices[10][2] = -H;
    vertices[11][0] = D;        vertices[11][1] = W;    vertices[11][2] = -H;

    vertices[12][0] = NoseX;        vertices[12][1] = -W;       vertices[12][2] = NoseZ;
    vertices[13][0] = xu[q];        vertices[13][1] = -W;       vertices[13][2] = H;
    vertices[14][0] = xu[Ni-1];     vertices[14][1] = -W;       vertices[14][2] = H;
    vertices[15][0] = D;            vertices[15][1] = -W;       vertices[15][2] = H;
    vertices[16][0] = 0.0;          vertices[16][1] = -W;       vertices[16][2] = 0.0;
    vertices[17][0] = xu[q];        vertices[17][1] = -W;       vertices[17][2] = zu[q];//error
    vertices[18][0] = xl[q];        vertices[18][1] = -W;       vertices[18][2] = zl[q];//error
    vertices[19][0] = xu[Ni-1];     vertices[19][1] = -W;       vertices[19][2] = zu[Ni-1];
    vertices[20][0] = D;            vertices[20][1] = -W;       vertices[20][2] = zu[Ni-1];
    vertices[21][0] = xl[q];        vertices[21][1] = -W;       vertices[21][2] = -H;
    vertices[22][0] = xu[Ni-1];     vertices[22][1] = -W;       vertices[22][2] = -H;
    vertices[23][0] = D;            vertices[23][1] = -W;       vertices[23][2] = -H;

    //Generamos el fichero blockMeshDict
    FILE *out;
    out = fopen("blockMeshDict","w");

    fprintf(out, "/*--------------------------------*- C++ -*----------------------------------*\\ \n");
    fprintf(out, "| =========                 |                                                 | \n");
    fprintf(out, "| \\\\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox           | \n");
    fprintf(out, "|  \\\\    /   O peration     | Version:  2.1.0                                 | \n");
    fprintf(out, "|   \\\\  /    A nd           | Web:      www.OpenFOAM.com                      | \n");
    fprintf(out, "|    \\\\/     M anipulation  |                                                 | \n");
    fprintf(out, "\\*---------------------------------------------------------------------------*/ \n");
    fprintf(out, "FoamFile                                                                        \n");
    fprintf(out, "{                                                                               \n");
    fprintf(out, "    version     2.0;                                                            \n");
    fprintf(out, "    format      ascii;                                                          \n");
    fprintf(out, "    class       dictionary;                                                     \n");
    fprintf(out, "    object      blockMeshDict;                                                  \n");
    fprintf(out, "}                                                                               \n");
    fprintf(out, "// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * // \n");
    fprintf(out, "\n");


    fprintf(out, "convertToMeters %f; \n", scale);
    fprintf(out, "\n");
    fprintf(out, "vertices \n");       
    fprintf(out, "( \n");
    for (int i = 0; i < 24; ++i)
    {
        fprintf(out, "      (%f %f %f)\n", vertices[i][0],vertices[i][1],vertices[i][2]);
    }
    fprintf(out, "); \n");
    fprintf(out, "\n");
    fprintf(out, "blocks \n");
    fprintf(out, "( \n");
    fprintf(out, "    hex (4 5 1 0 16 17 13 12)     (%d %d %d) edgeGrading (1 %f %f 1 %f %f %f %f 1 1 1 1) \n", Nleading, NT, NW, 1.0/ExpA, 1.0/ExpA, ExpT, ExpT, ExpT, ExpT);
    fprintf(out, "    hex (5 7 2 1 17 19 14 13)     (%d %d %d) simpleGrading (1 %f 1) \n", Ntrailing, NT, NW, ExpT);
    fprintf(out, "    hex (7 8 3 2 19 20 15 14)     (%d %d %d) simpleGrading (%f %f 1) \n", ND, NT, NW, ExpD, ExpT);
    fprintf(out, "    hex (16 18 21 12 4 6 9 0)     (%d %d %d) edgeGrading (1 %f %f 1 %f %f %f %f 1 1 1 1) \n", Nleading, NT, NW, 1.0/ExpA, 1.0/ExpA, ExpT, ExpT, ExpT, ExpT);
    fprintf(out, "    hex (18 19 22 21 6 7 10 9)    (%d %d %d) simpleGrading (1 %f 1) \n", Ntrailing, NT, NW, ExpT);
    fprintf(out, "    hex (19 20 23 22 7 8 11 10)   (%d %d %d) simpleGrading (%f %f 1) \n", ND, NT, NW, ExpD, ExpT);

    fprintf(out, "); \n");
    fprintf(out, "\n");
    
    fprintf(out, "edges \n");
    fprintf(out, "( \n");
    fprintf(out, "    spline 4 5 \n");
    fprintf(out, "        ( \n");
    for (int i = 1; i < q; ++i)
    {
        fprintf(out, "      (%f %f %f)\n", xu[i],W,zu[i]);//punto 1
    } 
    fprintf(out, "       ) \n");

    fprintf(out, "    spline 5 7 \n");
    fprintf(out, "        ( \n");
    for (int i = q+1; i < Ni-1; ++i)
    {
        fprintf(out , "     (%f %f %f)\n", xu[i],W,zu[i] );//punto 2

    }
    fprintf(out, "        ) \n");

    fprintf(out, "    spline 4 6 \n");
    fprintf(out, "        ( \n");
    for (int i = 1; i < q; ++i)
    {
        fprintf(out, "      (%f %f %f)\n", xl[i],W,zl[i] );//punto 3
    }
    fprintf(out, "        ) \n");

    fprintf(out, "    spline 6 7 \n");
    fprintf(out, "        ( \n");
    for (int i = q+1; i < Ni-1; ++i)
    {
        fprintf(out, "      (%f %f %f)\n", xl[i],W,zl[i] );//punto 4
    }
    fprintf(out, "        ) \n");

    fprintf(out, "    spline 16 17 \n");
    fprintf(out, "        ( \n");
    for (int i = 1; i < q; ++i)
    {
        fprintf(out, "      (%f %f %f)\n", xu[i],-W,zu[i]);//punto 5
    } 
    fprintf(out, "        ) \n");

    fprintf(out, "    spline 17 19 \n");
    fprintf(out, "        ( \n");
    for (int i = q+1; i < Ni-1; ++i)
    {
        fprintf(out , "     (%f %f %f)\n", xu[i],-W,zu[i] );//punto 6

    }
    fprintf(out, "        ) \n");

    fprintf(out, "    spline 16 18 \n");
    fprintf(out, "        ( \n");
    for (int i = 1; i < q; ++i)
    {
        fprintf(out, "      (%f %f %f)\n", xl[i],-W,zl[i] );//punto 7
    }
    fprintf(out, "        ) \n");

    fprintf(out, "    spline 18 19 \n");
    fprintf(out, "        ( \n");
    for (int i = q+1; i < Ni-1; ++i)
    {
        fprintf(out, "      (%f %f %f)\n", xl[i],-W,zl[i] );//punto 8
    }
    fprintf(out, "        ) \n");

    fprintf(out, "    arc 0 1 (%f %f %f) \n", -H*cos(pi/4)+xu[q],W,H*sin(pi/4));
    fprintf(out, "    arc 0 9 (%f %f %f) \n", -H*cos(pi/4)+xu[q],W,-H*sin(pi/4));
    fprintf(out, "    arc 12 13 (%f %f %f) \n", -H*cos(pi/4)+xu[q],-W,H*sin(pi/4));
    fprintf(out, "    arc 12 21 (%f %f %f) \n", -H*cos(pi/4)+xu[q],-W,-H*sin(pi/4));

    fprintf(out, "); \n");
    fprintf(out, "\n");
    fprintf(out, "boundary \n");
    fprintf(out, "( \n");

    fprintf(out, "    inlet \n");
    fprintf(out, "    { \n");
    fprintf(out, "        type patch; \n");
    fprintf(out, "        faces \n");
    fprintf(out, "        ( \n");
    fprintf(out, "            (1 0 12 13) \n");
    fprintf(out, "            (0 9 21 12) \n");
    fprintf(out, "        ); \n");
    fprintf(out, "    } \n");
    fprintf(out, "\n");

    fprintf(out, "    outlet \n");
    fprintf(out, "    { \n");
    fprintf(out, "        type patch; \n");
    fprintf(out, "        faces \n");
    fprintf(out, "        ( \n");
    fprintf(out, "            (11 8 20 23) \n");
    fprintf(out, "            (8 3 15 20) \n");
    fprintf(out, "        ); \n");
    fprintf(out, "    } \n");
    fprintf(out, "\n");

    fprintf(out, "    topAndBottom \n");
    fprintf(out, "    { \n");
    fprintf(out, "        type patch; \n");
    fprintf(out, "        faces \n");
    fprintf(out, "        ( \n");
    fprintf(out, "            (3 2 14 15) \n");
    fprintf(out, "            (2 1 13 14) \n");
    fprintf(out, "            (9 10 22 21) \n");
    fprintf(out, "            (10 11 23 22) \n");
    fprintf(out, "        ); \n");
    fprintf(out, "    } \n");
    fprintf(out, "\n");

    fprintf(out, "    naca4 \n");
    fprintf(out, "    { \n");
    fprintf(out, "        type wall; \n");
    fprintf(out, "        faces \n");
    fprintf(out, "        ( \n");
    fprintf(out, "            (5 4 16 17) \n");
    fprintf(out, "            (7 5 17 19) \n");
    fprintf(out, "            (4 6 18 16) \n");
    fprintf(out, "            (6 7 19 18) \n");
    fprintf(out, "        ); \n");
    fprintf(out, "    } \n");
    fprintf(out, "); \n");
    fprintf(out, " \n");
    fprintf(out, "mergePatchPairs \n");
    fprintf(out, "( \n");
    fprintf(out, "); \n");
    fprintf(out, " \n");
    fprintf(out, "// ************************************************************************* // \n");

    fclose(out);

    cout << "----> Generated blockMeshDict" << endl;
}

void naca4::exportCoordinates()
{
    FILE *out;
    out = fopen("output/coordinates.dat","w");
    for (int i = 0; i < Ni; ++i)
    {
        fprintf(out, "%f %f %f %f\n", xu[i],zu[i],xl[i],zl[i]);
    }
    fclose(out);
    //system("mv coordinates.dat coordinates");

    /*
    int output = system ("gnuplot plotnaca4.gp");
    if( output != 0)
    {
        cout << "Error, algo ha salido mal con GNUPlot" << endl;
    }
    else{
        cout << "No hay problemas con el comando GnuPlot." << endl;
    }
    */
}

void naca4::plot()
{
    int output = system("cd output && gnuplot -persist plotNaca4.gp && cd ..");
}
