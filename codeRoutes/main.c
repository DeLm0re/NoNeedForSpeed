
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
#include "region.h"

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
		
		//////////////////////////
	    //--- line detection ---//
	    //////////////////////////
		
		// We create the hough transform of the image
		printf("Creating the Hough transformation of the image\n");
		DonneesImageTab* tabHough = createHough(tabImage, 200, 720);
		printf(" Edit it so it is easier to use\n");
        //Making every point black or white
		cutBetweenLevel(tabHough, 80, 255);
		cutBetweenLevel(tabHough, 0, 80);
		//Making sure to have a blob around each point to find the regions easily with a dillatation filter
		applyDillatationFilter(tabHough, 200);
		
		// We find each point on the hough transform by finding regions using the bottom up method
		printf("Creating a region for each lines on the hough transform \n");
		DonneesImageTab* tabRegionHough = initTabRegion(tabHough->largeurImage, tabHough->hauteurImage);
		IdRegions* idRegionsHough = findAllRegionBottomUp(tabHough, tabRegionHough, 200);
		printf(" line founds : %d\n", idRegionsHough->size-1);
		
		printf("Finding the two lines and their crossing point\n");
		Line* line1 = getCenterLineFromRegion(tabHough, tabRegionHough, idRegionsHough->regions[1], 0);
        Line* line2 = getCenterLineFromRegion(tabHough, tabRegionHough, idRegionsHough->regions[2], 0);
        Point* point = getCrossingPoint(line1, line2);
        printf(" Crossing point found : (%d, %d)\n", point->x, point->y);
		
		/////////////////////////////
	    //--- memory liberation ---//
	    /////////////////////////////
	    
		printf("Freeing the memory\n");
		libereDonneesImageRGB(&image);
		
		libereDonneesTab(&tabImage);
		
		libereDonneesTab(&tabHough);
		
		libereDonneesTab(&tabRegionHough);
		destructIdRegions(&idRegionsHough);
		free(line1);
		free(line2);
		
		free(point);
		
	}
	else
	{
		printf("Error : Can't read the image\n");
	}
	printf("---Done\n");
	return 0;
}
