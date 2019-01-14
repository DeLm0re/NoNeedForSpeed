
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
#include "road.h"

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

		printf("Creating the matrice from the image\n");
		DonneesImageTab* tabImage = RGBToTab(image);
		printf(" Creating the image wiewed by the retina\n");
		DonneesImageRGB* newImage = tabToRGB(tabImage);
		ecrisBMPRGB_Dans(newImage, "newImage.bmp");

		/////////////////////////////
	    //--- memory liberation ---//
	    /////////////////////////////

		printf("Freeing the memory\n");
		libereDonneesImageRGB(&image);

		libereDonneesTab(&tabImage);
		libereDonneesImageRGB(&newImage);
	}
	else
	{
		printf("Error : Can't read the image\n");
	}
	printf("---Done\n");
	return 0;
}
