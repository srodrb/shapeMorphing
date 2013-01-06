#include "common.h"

char *C, cad[300];

char* readPath()
{
    // Funcion que lee la ruta actual en forma de cadena
    FILE *fp;

    fp=fopen(".path.txt", "r");

    while(C=fgets(cad,300,fp))
        //printf("%s",cad);
    
    return cad;
};




char* setPath(const char* route,const char* filename)
{

    long size = 1000;
    char *ptr;
    char *buf;

    if ( (buf = (char*)malloc(size*sizeof(char))) != NULL)
    {
        ptr = getcwd(buf, size*sizeof(char) );
    }
    //char changedir[1000] = "cd ";
    char OFpath[100] = "/caseOF/";
    //strcat(changedir,ptr);
    strcat(ptr,OFpath);
    strcat(ptr,route);
    strcat(ptr,"/");
    strcat(ptr,filename);
    //cout << changedir << endl;
    cout << ptr << endl;
    

    return ptr;
}