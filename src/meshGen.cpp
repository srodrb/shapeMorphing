#include "meshGen.h"

meshGen::meshGen(naca4parameters parameters): naca4(parameters)
{
    printf("Creamos la malla a partir del perfil naca de 4 cifras\n");
    generateBlockMeshDict();
    printf("BlockMeshDict generated!\n");
}

void meshGen::generateBlockMeshDict()
{
    //_____Virtual wind tunnel dimensions_______//
    const float H = 8.0;
    const float W = 0.5;
    const float D = 16.0;
    //____Mesh density control parameters______//
    const int Nx = 150;
    const int ND = 100;
    const int NT = 80;
    const int NW = 1;
    //____Mesh expansion ratios________________//
    const float ExpT = 5;
    const float ExpD = 15;
    const float ExpA = 5;
    scale = 1.0; 
    //:::::::::::::::::::::::::::::::::::::::://
    

    float NoseX =  (-H+xu[q])*cos(alpha); //cout << "NoseX value: " << NoseX << endl;
    float NoseZ = -(-H+xu[q])*sin(alpha); //cout << "NoseZ value: " << NoseZ << endl;
    
    //int Nleading  = ceil(0.35*Nx);//TODO alterado, era 0.3*Nx
    //int Ntrailing = Nx-Nleading;

    /*
     * Se trata de estar mas cerca del borde de entrada teniendo mas divisiones.
     * Cuando mas cerca estemos mejor se adapta la malla a la superficie curva de entrada.
     */
    int Nleading = 40;        //divisiones hacia el borde de entrada
    int Ntrailing = 70;       //divisiones hacia el borde de salida
    q = 35;                   //cuanto menor sea mas cerca estoy del borde de entrada (leading)
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
    int output = system("rm -r /home/samuel/workSpace/shapeMorphing/OFcase/constant/polyMesh/*");//para hacer las cosas un poco mas rapido
    out = fopen("/home/samuel/workSpace/shapeMorphing/OFcase/constant/polyMesh/blockMeshDict","w");//creo el fichero en mi ruta

    fprintf(out, "/*--------------------------------*- C++ -*----------------------------------*\\ \n");
    fprintf(out, "| ==========                  |                                                 | \n");
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
    fprintf(out, "    hex (4 5 1 0 16 17 13 12)     (%d %d %d) edgeGrading (0.05 %f %f 0.05 %f %f %f %f 1 1 1 1) \n", Nleading, NT, NW, 1.0/ExpA, 1.0/ExpA, ExpT, ExpT, ExpT, ExpT);
    //fprintf(out, "    hex (4 5 1 0 16 17 13 12)     (%d %d %d) edgeGrading (1 %f %f 1 %f %f %f %f 1 1 1 1) \n", Nleading, NT, NW, 1.0/ExpA, 1.0/ExpA, ExpT, ExpT, ExpT, ExpT);
    fprintf(out, "    hex (5 7 2 1 17 19 14 13)     (%d %d %d) simpleGrading (1 %f 1) \n", Ntrailing, NT, NW, ExpT);
    fprintf(out, "    hex (7 8 3 2 19 20 15 14)     (%d %d %d) simpleGrading (%f %f 1) \n", ND, NT, NW, ExpD, ExpT);
    fprintf(out, "    hex (16 18 21 12 4 6 9 0)     (%d %d %d) edgeGrading (0.05 %f %f 0.05 %f %f %f %f 1 1 1 1) \n", Nleading, NT, NW, 1.0/ExpA, 1.0/ExpA, ExpT, ExpT, ExpT, ExpT);
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
