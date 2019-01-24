
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

int main(int argc, char** argv)
{
	// Initializing the random with a seed
	srand(10495854);
	char pathStepImage [50]; // use to store the path of the image that visualize each step

    DonneesImageRGB* imageSign = lisBMPRGB("imagesRoutes/panneau4.bmp");
    DonneesImageTab* imageSignTab = RGBToTab(imageSign);
    
    // Filters
    DonneesImageTab* imageSignTabMedian = applyMedianFilterOnTab(imageSignTab, 5, 5);
	
	// paneau : (0, 140, 120) (110, 110, 110) (140, 90, 10)
	DonneesImageTab* tabRegion = initTabRegion(imageSign->largeurImage, imageSign->hauteurImage);
	IdRegions* idRegions = findAllColorRegionBottomUp(imageSignTabMedian, tabRegion, 0, 140, 120, 50);
	printf("number of regions : %d\n", idRegions->size);
	
	DonneesImageRGB* imageRegion = tabToRGB(tabRegion);
	ecrisBMPRGB_Dans(imageRegion, "1 - tabRegion.bmp");
	
	DonneesImageRGB* imageRef = lisBMPRGB("imagesRoutes/panneauExtrait1.bmp");
    DonneesImageTab* imageRefTab = RGBToTab(imageRef);
    makeGreyLevel(imageRefTab);
    DonneesImageTab* imageRefTabLBP = applyLocalBinaryPattern(imageRefTab);
	DonneesImageTab* currentShape = NULL;
	DonneesImageTab* LBPShape = NULL;
	
	DonneesImageRGB* imageRegionShape = NULL;
	float error = 0;
	int i;
	for(i = 0; i < idRegions->size; i++)
	{
	    currentShape = getOriginalShape(imageSignTab, tabRegion,  idRegions->regions[i]);
	    makeGreyLevel(currentShape);
	    // Need a better getShape that get what is on the image
	    LBPShape = applyLocalBinaryPattern(currentShape);
	    error = binaryPatternComp(LBPShape, imageRefTabLBP);
	    printf("%d testComp : %f\n", i, error);
	    if (error <= 0.0001)
	    {
	        // We reset the path of the step visualization image
	        strcpy(pathStepImage, "");
	        // We create the new one
	        sprintf(pathStepImage, "panneaux/%d - sign.bmp", i);
	        imageRegionShape = tabToRGB(currentShape);
	        ecrisBMPRGB_Dans(imageRegionShape, pathStepImage);
	    }
	    libereDonneesTab(&currentShape);
	    libereDonneesTab(&LBPShape);
	}
    
    libereDonneesImageRGB(&imageSign);
    libereDonneesTab(&imageSignTab);
    
    libereDonneesTab(&imageSignTabMedian);
    
    libereDonneesTab(&tabRegion);
    destructIdRegions(&idRegions);
    
    libereDonneesImageRGB(&imageRef);
    libereDonneesTab(&imageRefTab);
    
	return 0;
}
