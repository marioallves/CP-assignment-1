/* ---------------------------- */
/* Joao Lourenco                */
/* joao.lourenco@fct.unl.pt     */
/* DI-FCT-UNL 2015              */
/* ---------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <cilk/cilk.h>
#include "ppm.h"

#define XCENTER -2
#define YCENTER -1.5
#define SCALE 0.003
#define COLORMAP "mca.map"

unsigned char *matrix;

int ma(double ax, double ay){
    double a1,b1,a2,b2;
    a1=ax; b1=ay; 
    int lp=0;
            while ((lp<=255) && (a1*a1+b1*b1<=4))
            {
                lp++;
                a2=a1*a1-b1*b1+ax;
                b2=2*a1*b1+ay; 
                a1=a2; 
                b1=b2;
            }
            if (lp>255)
                lp=0;
    return lp;
}


int main (int argc, char **argv)
{
    int x,y,lp;
    double ax,ay;
    
    unsigned long int index;
    int imgsize;
    double scale;

    /* help */
    if (argc!=3)
    {
        printf ("%s <imgsize> <file.ppm>\n\n", argv[0]);
        exit(0);
    }
  
    /* setup */
    imgsize = atoi (argv[1]);
    assert (imgsize >= 1024);
    scale = SCALE * 1024 / imgsize;
    matrix = (unsigned char*) calloc (imgsize*imgsize, sizeof (unsigned char));
    assert (matrix != NULL);
  
    /* mandelbrot */
    cilk_for (x=0; x<imgsize-1; x++)
        for (y=0; y<imgsize-1; y++)
        {
            ax=XCENTER+x*scale;
            ay=YCENTER+y*scale;
            
            index = x+(y*imgsize);
            matrix[index]=ma(ax,ay);
        }

    /* output to file */
    output_ppm (argv[2], COLORMAP, matrix, imgsize, imgsize, 255);

    free (matrix);
    return 1;
}
