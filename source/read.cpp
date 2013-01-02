/*
 * =====================================================================================
 *
 *       Filename:  read.cpp
 *
 *    Description:          
 *
 *        Version:  1.0
 *        Created:  31/12/12 12:00:15
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Samuel Rodriguez Bernabeu (srodrb), srodrb@gmail.com
 *   Organization:  University of Leon (Spain)
 *
 * =====================================================================================
 */

#include <stdio.h>

int main()
{
    FILE *fp;
    char *c, cad[300];

    fp=fopen(".path.txt", "r");

    while(c=fgets(cad,100,fp))
        printf("%s",cad);

    //getchar();
    return 0;
}




