
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
#include "neuralNetwork.h"

int main(void)
{
	// Initializing the random with a seed
	srand(10495854);

	DonneesImageRGB *imageCouleur = lisBMPRGB("test.bmp");

	DonneesImageTab *imageCouleurTab = RGBToTab(imageCouleur);

	DonneesImageTab *imageRegion = initTabRegion(imageCouleur->largeurImage, imageCouleur->hauteurImage);

	IdRegions *tableauRegion = findAllRegionBottomUp(imageCouleurTab, imageRegion, 200);

	writeAllRegion(imageRegion, tableauRegion);

	libereDonneesTab(&imageCouleurTab);
	libereDonneesTab(&imageRegion);
	libereDonneesImageRGB(&imageCouleur);
	destructIdRegions(&tableauRegion);

	return 0;
}
