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

/*
int main(int argc, const char *argv[])
{
    printf("Main module... Menu isn't implemented yet!! \n");
    
    //_______________________________________Airfoil parameters
    float m = 4.0;
    float p = 4.0;
    float t = 12.0;
    float c = 1.0; 

    naca4parameters parameters(c,m,p,t,0.0,0.0);
    //meshGen naca7512 (parameters);
    naca4 foil(parameters);
    splineShape shape(4,parameters);            

    shape.plot();
    displacementStruct displacement(315.0,0.05);
    int pointID = 11;
    shape.modifyControlPoint(pointID, displacement);
    shape.plot();
    
    shape.modifyControlPoint(pointID, displacement);
    shape.plot();

    //Una vez hemos terminado de hacer modificaciones sobre los perfiles generamos la malla
    shape.meshGen();
    
    int output = system ("cd ../OFcase && blockMesh && checkMesh && paraFoam" );

    return 0;
}*/


const char* program_name;
void print_usage (FILE* stream, int exit_code) 
{
  fprintf (stream, "Usage: %s options [ inputfile ... ]\n", program_name); 
  fprintf (stream,
          "Basic options:\n"
            "\t -h --help Display this usage information.\n" 
            "\t -o --output filename Write output to file.\n"
            "\t -v --verbose Print verbose messages.\n"
         "Configuration options:\n"
            "\t -p --Set OpenFoam case path.\n"
            "\t -P --Display actual path for blockMeshDict.\n"
            "\t -g --Shows airfoil plot with Gnuplot.\n"
         "Mesh generation options:\n"
            "\t -N --Naca 4 digits series airfoils.\n"
         "Advanced options (beta):\n"
            "\t -S --Airfoil shape optimization.\n"
         "Postprocessing options:\n"
            "\t -c --Choose OpenFOAM solver (not implemented).\n"
            "\t -C --Generate OpenFOAM case on specified path.\n"
            "\t -r --Run case with OpenFOAM solver.\n"
            "\t -m --Build mesh, check and refine..\n");
  exit (exit_code); 
}
/* Main program entry point. ARGC contains number of argument list elements; ARGV is an array of pointers to them. */
int main (int argc, char* argv[]) 
{

    //TODO vamos con las pruebas:
    char nombre[] = "Gema";
    char apellido[] = "Zaragoza";
    strcat(nombre,apellido);
    cout << "nombre: " << nombre << " apellido " << apellido << endl;

  int next_option;
  /* A string listing valid short options letters. */ 
  const char* const short_options = "hp:Pgo:vNSm";
  /* An array describing valid long options. */
  const struct option long_options[] = 
  {
        { "help"        , 0, NULL, 'h' }, 
        { "pathset"     , 1, NULL, 'p' },
        { "pathdisplay" , 0, NULL, 'P' },
        { "output"      , 1, NULL, 'o' }, 
        { "verbose"     , 0, NULL, 'v' },
        { "graph"       , 0, NULL, 'g' },
        { "Naca4"       , 0, NULL, 'N' },
        { "ShapeOpt"    , 0, NULL, 'S' },
        { "Mesh"        , 0, NULL, 'm' },
        { NULL          , 0, NULL,  0  } /* Required at end of array. */ 
  };
/* The name of the file to receive program output, or NULL for standard output. */
const char* output_filename = NULL;
/* Whether to display verbose messages. */ 
int verbose = 0;
program_name = "CMeshFoil"; //Program name
bool plotOption;            //Plot option

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
/* This option takes an argument, the name of the output file. */ 
      output_filename = optarg;
      break;
    case 'v': /* -v or --verbose */ 
      verbose = 1;
      break;

    //================================================================ MESH GENERATION OPTIONS =======//
    case 'g':
      {
          plotOption = true;
      }
      break;

    case 'N'://NACA 4 series
      {

        float m,p,t;
        cout << "\tMaximum camber (m) value?             "; cin >> m;
        cout << "\tPosition of maximum camber (p) value? "; cin >> p;
        cout << "\tMaximum thickness value (t) value?    "; cin >> t;

        // Create an structure witch contain all paremeters needed.
        naca4parameters parameters(1.0,m,p,t,0.0,0.0);
        // Create airfoil
        naca4 airfoil(parameters);
        
        if(plotOption == true)
        {
            airfoil.plot();
        }
        
        airfoil.meshGen();
        printf("BlockMeshDict has been created on default path.\n");
      }
      break;

    case '?': /* The user specified an invalid option. */
              /* Print usage information to standard error, and exit with exit
              code one (indicating abnormal termination). */ 
      print_usage (stderr, 1);

    case 'S':
      {
          float m,p,t;
         cout << "\tMaximum camber (m) value?             "; cin >> m;
         cout << "\tPosition of maximum camber (p) value? "; cin >> p;
         cout << "\tMaximum thickness value (t) value?    "; cin >> t;

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
        int order = system ("blockMesh && checkMesh && paraFoam"); //refineMesh is optional
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

if (verbose) 
{ 
  int i;
  for (i = optind; i < argc; ++i) printf ("Argument: %s\n", argv[i]);
}
/* The main program goes here. */
return 0;

}
