
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
#include "roadDetection.h"

int main(int argc, char** argv)
{
	//Initialize random with a seed
	printf("Initializing random with a seed\n");
	srand(10495854);
	

	// We get the image we will use for the test
	printf("Reading the image\n");
	DonneesImageRGB* image = NULL;
	if (argc != 2)
	{
		image = lisBMPRGB("test.bmp");
	}
	else
	{
		image = lisBMPRGB(argv[1]);
	}
	if (image != NULL)
	{
	    
	    /////////////////////////
	    //--- preprocessing ---//
	    /////////////////////////
	    
		printf("Creating a matrice of pixels from the image\n");
		DonneesImageTab* tabImage = RGBToTab(image);
		
		///////////////////////////
	    //--- point detection ---//
	    ///////////////////////////

        Point* point = getRoadPoint(tabImage);
        printf(" Crossing point found : (%d, %d)\n", point->x, point->y);
		
		/////////////////////////////
	    //--- memory liberation ---//
	    /////////////////////////////
	    
		printf("Freeing the memory\n");
		libereDonneesImageRGB(&image);
		
		libereDonneesTab(&tabImage);
		
		free(point);
		
	}
	else
	{
		printf("Error : Can't read the image\n");
	}
	printf("---Done\n");
	return 0;
}
