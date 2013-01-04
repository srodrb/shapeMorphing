#include "common.h"

char *C, cad[300];

char* readPath()
{
    FILE *fp;

    fp=fopen(".path.txt", "r");

    while(C=fgets(cad,300,fp))
        //printf("%s",cad);
    
    return cad;
};

void changeToActualDir()
{
    long size = 1000;
    char *ptr;
    char *buf;

    if ( (buf = (char*)malloc(size*sizeof(char))) != NULL)
    {
        ptr = getcwd(buf, size*sizeof(char) );
    }
    char changedir[1000] = "cd ";
    strcat(changedir,ptr);
    cout << changedir << endl;
    cout << ptr << endl;
    system(changedir);
    
    ptr = getcwd(buf,size*sizeof(char));
    cout << ptr << endl;
};

char* BlockMeshDictPath()
{

    long size = 1000;
    char *ptr;
    char *buf;

    if ( (buf = (char*)malloc(size*sizeof(char))) != NULL)
    {
        ptr = getcwd(buf, size*sizeof(char) );
    }
    //char changedir[1000] = "cd ";
    char OFpath[100] = "/caseOF/constant/polyMesh/blockMeshDict";
    //strcat(changedir,ptr);
    strcat(ptr,OFpath);
    //cout << changedir << endl;
    cout << ptr << endl;
    

    return ptr;
}
