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

void createOFcase()
{

    int order = system("mkdir constant && mkdir constant/polymesh");
};
