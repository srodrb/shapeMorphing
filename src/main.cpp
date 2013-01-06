/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  Fichero principal del proyecto CMeshFoil.       
 *
 *        Version:  1.0
 *        Created:  26/12/12 08:39:00
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Samuel Rodriguez Bernabeu (srodrb), srodrb@gmail.com
 *   Organization:  University of Leon (Spain)
 *
 * =====================================================================================
 */

#include "common.h"
#include "data.h"

#include "meshGen.h"

#include "parameterStructs.h"

#include "splineShape.h"
#include "naca4.h"


const char* program_name;
void print_usage (FILE* stream, int exit_code) 
{
  fprintf (stream, "\n\tUsage: %s options [ inputfile ... ]\n", program_name); 
  fprintf (stream, "\t       [Configuration][Mesh][Advanced][Postprocessing]\n\n");
  fprintf (stream,
          "\n\tBasic options:\n"
          "\t_________________________________________________________________\n"
            "\t\t -h --help Display this usage information.\n" 
            "\t\t -o --output filename Write output to file.\n"
         "\n\tConfiguration options:\n"
         "\t_________________________________________________________________\n"
            "\t\t -g --Shows airfoil plot with Gnuplot.\n"
         "\n\tMesh generation options:\n"
            "\t\t -N --Naca 4 digits series airfoils.\n"
         "\n\tAdvanced options (beta):\n"
         "\t_________________________________________________________________\n"
            "\t\t -S --Airfoil shape optimization.\n"
            "\t\t      (Not implemented in this version of cmesfoil).\n"
            "\t\t      See documentation for futher information and\n"
            "\t\t      development status.\n"
         "\n\tPostprocessing options:\n"
         "\t_________________________________________________________________\n"
            "\t\t -c --Choose OpenFOAM solver (not implemented).\n"
            "\t\t      (Not implemented in this version of cmesfoil).\n"
            "\t\t      See documentation for futher information and\n"
            "\t\t      development status.\n"
            "\t\t -m --Build mesh, check and refine.\n"
            "\t\t -V --View the mesh with paraView.\n\n\n");
  exit (exit_code); 
}

/* Main program entry point. ARGC contains number of argument list elements; ARGV is an array of pointers to them. */
int main (int argc, char* argv[]) 
{
  int next_option;
  /* A string listing valid short options letters. */ 
  const char* const short_options = "hgo:NSmV";
  /* An array describing valid long options. */
  const struct option long_options[] = 
  {
        { "help"        , 0, NULL, 'h' }, 
        { "output"      , 1, NULL, 'o' }, 
        { "graph"       , 0, NULL, 'g' },
        { "Naca4"       , 0, NULL, 'N' },
        { "ShapeOpt"    , 0, NULL, 'S' },
        { "Mesh"        , 0, NULL, 'm' },
        { "View"        , 0, NULL, 'V' },
        { NULL          , 0, NULL,  0  } /* Required at end of array. */ 
  };

  const char* output_filename = NULL;
program_name = "CMeshFoil"; //Program name
bool plotOption = false;            //Plot option

do {
  next_option = getopt_long (argc, argv, short_options,
  long_options, NULL);
  switch (next_option)
  {
    case 'h': /* -h or --help */
              /* User has requested usage information. Print it to standard output, and exit with exit code zero (normal termination). */
      print_usage (stdout, 0);

    case 'p':
      {
          printf("Please write the OpenFOAM case complete path (p.e: /home/user/case/): \n");
          //TODO tengo que hacer que cuando se introduzca la ruta el programa se cierre o reinicie el loop
          //     tambien tiene que leer la ruta al principio!!
      }
      printf("Not implemented yet\n");
      break;
    case 'P':
      {
          printf("Actual path is: %s\n", readPath());
      }
      break;
    case 'o': /* -o or --output */
      output_filename = optarg;
      break;

    /*::::::::::::::::::::::::::::: MESH GENERATION OPTIONS ::::::::::::::::::::::::::::::::::*/
    case 'g': /* -g or --graph */
      {
          plotOption = true;
      }
      break;

    case 'N': /* -N or --Naca4 */
      {

        float m,p,t;
        printf ("\tInfo: please give naca 4 series parameters to the code.\n");
        cout << "\t     Maximum camber (m) value?             "; cin >> m;
        cout << "\t     Position of maximum camber (p) value? "; cin >> p;
        cout << "\t     Maximum thickness value (t) value?    "; cin >> t;

        naca4parameters parameters(1.0,m,p,t,0.0,0.0);
        naca4 airfoil(parameters);
        
        if(plotOption == true)
        {
            airfoil.plot();
        }
        
        airfoil.meshGen();
        printf("BlockMeshDict has been created.\n");
        airfoil.sampleDictGen();
        printf("sampleDict file has been created\n");
      }
      break;

    case '?': /* The user specified an invalid option. */
              /* Print usage information to standard error, and exit with exit
              code one (indicating abnormal termination). */ 
      print_usage (stderr, 1);

    case 'S': /* -S or --ShapeOpt */
      {
        /* Naca 4 series airfoil variables */  
        float m,p,t;
        printf ("\tInfo: please give naca 4 series parameters to the code.\n");
        cout << "\t      Maximum camber (m) value?             "; cin >> m;
        cout << "\t      Position of maximum camber (p) value? "; cin >> p;
        cout << "\t      Maximum thickness value (t) value?    "; cin >> t;

         naca4parameters parameters(1.0,m,p,t,0.0,0.0);
         splineShape airfoilShape(parameters);
         printf("spline created!\n");
         
         if(plotOption == true)
         {
            airfoilShape.plot();
         }
        displacementStruct displacement(315.0,0.05);
        int pointID = 11;
        airfoilShape.modifyControlPoint(pointID, displacement);
        airfoilShape.plot();
         
      }

    case 'm':
      {
          // Build mesh with blockMesh command and check
          // and refine with checkMesh and refineMesh.
        printf("\tInfo: 1.-Creating a new case structure, just for meshing.\n");
        printf("\tInfo: 2.-Creating blockMeshDict file to caseOF/constant/polyMesh\n");
        printf("\tInfo: 3.-Running blockMesh and checkMesh commands on caseOF/constant/polyMesh\n");
        system("cd caseOF/ && blockMesh && checkMesh"); //refineMesh is optional
        printf("\tInfo: 4.-Done. Now you can copy caseOF/constant/polyMesh files into your case directory.\n");
        system("cd ..");
        system("sh /opt/cmeshfoil/scripts/setIcoFoam.sh");
      }
      break;

    case 'V':
    {
      printf("\tInfo: Opening mesh with paraView\n");
      system("cd caseOF && paraFoam");
      system("cd ..");
    }
    break;
    
    case -1: 
      break;
    
    default: 
      abort ();
} 
}/* Done with options. *//* Something else: unexpected. */
while (next_option != -1);
/* Done with options. OPTIND points to first nonoption argument. For demonstration purposes, print them if the verbose option was specified. */

return 0;

}
