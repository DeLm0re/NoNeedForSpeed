
//General header files
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//More specific headers files
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

//Header file
#include "road.h"


int main(int argc, char** argv)
{
	//Initialize random with a seed
	printf("Initializing random with a seed\n");
	srand(10495854);
		
	/////////////////////////
	//--- road creation ---//
	/////////////////////////
	
	Car carPosition;
	carPosition.abscissa = 150;
	carPosition.directionAngle = M_PI/2;
	DonneesImageTab* roadTab = createRoadImage(150, 200, carPosition, 150, 4, 300);
	DonneesImageTab* roadTabGradiant = applyGradiantFilterOnTab(roadTab, PREWITT);
	
	///////////////////////////
    //--- point detection ---//
    ///////////////////////////

    Point* point = getRoadPoint(roadTabGradiant);
    printf(" Crossing point found : (%d, %d)\n", point->x, point->y);
	
	/////////////////////////////
    //--- memory liberation ---//
    /////////////////////////////
    
	printf("Freeing the memory\n");
	libereDonneesImageRGB(&image);
	
	libereDonneesTab(&roadTab);
	libereDonneesTab(&roadTabGradiant);
	
	libereDonneesTab(&tabImage);
	
	free(point);
	return 0;
}
