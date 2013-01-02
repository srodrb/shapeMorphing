/*
 * =====================================================================================
 *
 *       Filename:  Cmesh.cpp
 *
 *    Description:  Fichero principal del programa CFoilMesh.
 *                  El programa genera una malla a partir de la designacion del perfil
 *
 *        Version:  1.0
 *        Created:  04/12/2012 11:49:37
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Samuel Rodriguez Bernabeu
 *   Organization:  Universidad de León, Spain.
 *
 * =====================================================================================
 */

#include "common.h"
#include "airfoil.h"

/* The name of this program.  */
const char* program_name;

/* Prints usage information for this program to STREAM (typically
   stdout or stderr), and exit the program with EXIT_CODE.  Does not
   return.  */

void print_usage (FILE* stream, int exit_code)
{
  fprintf (stream, "--------------------------------------------------------------\n");
  fprintf (stream, "                  BASIC USAGE                                 \n");
  fprintf (stream, "Usage:  %s options [ inputfile ... ]\n\n", program_name);
  fprintf (stream,
           "  -h  --help             Display this usage information.\n"
           "  -i  --info             /*Not yet implemented*/ Display extended information.\n"
           "  -o  --output filename  Write output to file.\n"
           "  -e  --export           Save airfoil coordinates to a file.\n"//TODO
           "  -p  --plot             Plot airfoil through GNUPlot.\n"//TODO
           "                         Save and image capture.\n"
           "  -v  --verbose          Print verbose messages.\n");
  fprintf (stream, "--------------------------------------------------------------\n");
  fprintf (stream, "                  SUPPORTED AIRFOILS                       \n");
  fprintf (stream,
           "                                                 NACA 4 digits series.\n"
           "/*Not yet implemented*/                          NACA 5 digits series.\n");

  fprintf (stream, "--------------------------------------------------------------\n");
  exit (exit_code);
}

void print_extended_info ()
{
    /*
     * Uses 'more' program to display info.txt file
     */

    system("more -c -e info.txt");
}

/* Main program entry point.  ARGC conains number of argument list
   elements; ARGV is an array of pointers to them.  */

int main(int argc, char *argv[])
{

    int next_option;
    bool out_plot = false;
    bool out_coordinates = false;

  /* A string listing valid short options letters.  */
  const char* const short_options = "hio:e:pv";

  /*
   * El array describle las optiones, 0 si no necesita argumentos, 1 si lo necesita
   */
  const struct option long_options[] = {
    { "help",     0, NULL, 'h' },
    { "info",     0, NULL, 'i' },
    { "output",   1, NULL, 'o' },
    { "export",   1, NULL, 'e' },
    { "plot",     0, NULL, 'p' },
    { "verbose",  0, NULL, 'v' },
    { NULL,       0, NULL, 0   }   /* Required at end of array.  */
  };

  const char* output_filename = NULL;
  int verbose = 0;
  program_name = argv[0];

  do {
    next_option = getopt_long (argc, argv, short_options,
                               long_options, NULL);
    switch (next_option)
    {
    case 'h':
      print_usage (stdout, 0);

    case 'i':
        print_extended_info();
        exit(-1);

    case 'o':
        output_filename = optarg;
        break;

    case 'e':
        output_filename = optarg;
        out_coordinates = true;
        break;

    case 'p':
        if ( output_filename == NULL)//does the file exist?
        {
            //printf("El fichero no existe, por favor exporte primero los datos\n");
            exit(-1);
        }

        //printf("tenemos el fichero de datos, vamos a mostrarlo...\n");
        //debug_printf("Definimos el preprocesador para plot\n");
        out_plot = true;
        break;

    case 'v':   /* -v or --verbose */
      verbose = 1;
      break;

    case '?':   /* The user specified an invalid option.  */
      /* Print usage information to standard error, and exit with exit
         code one (indicating abonormal termination).  */
      print_usage (stderr, 1);
      

    case -1:    /* Done with options.  */
      break;

    default:    /* Something else: unexpected.  */
      abort ();
    }
  }
  while (next_option != -1);

    //main loop

  int foilType = menu();
  switch (foilType)
  {
    case 0:
        new_option:
        char confirmation;
        float m,p,t;

        printf("NACA 4 digits series airfoil selected, please specify values below\n");
        cout << "m value: "; cin >> m;
        cout << "p value: "; cin >> p;
        cout << "t value: "; cin >> t;
        cout << "Selected airfoil is NACA " << m << p << t << " continue? (yes/no). Any key for exit.";
        cin >> confirmation;

        if (confirmation == 'y'){
            airfoil foil(1,m,p,t,100);
            foil.coordinates();
            foil.meshGen();
            break;
        }
        else if(confirmation == 'n'){
          cout << "Back to main menu..." << endl;
          system ("rm blockMeshDict");
          goto new_option;
        }
        else{
          cout << "Non correct option."  << endl;
          exit(-1);
        }

    default:
          exit(-1);
  }



    /*
    if ( out_coordinates )
    {
        AIRFOIL.exportCoordinates( output_filename );
    }

    if (out_plot)
    {
        AIRFOIL.outPlot();
    }
    */
    return 0;
}
