
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
#include "motif.h"


int main(int argc, char** argv)
{
	// Initializing the random with a seed
	srand(10495854);

    DonneesImageRGB* imageSign = lisBMPRGB("imagesRoutes/panneau1.bmp");
    DonneesImageTab* imageSignTab = RGBToTab(imageSign);
    
    
    
    libereDonneesImageRGB(&imageSign);
    libereDonneesTab(&imageSignTab);
    
	return 0;
}
