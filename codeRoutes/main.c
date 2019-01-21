
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
#include "trajectory.h"


int main(int argc, char** argv)
{
	// Initialize random with a seed
	printf("Initializing random with a seed\n");
	srand(10495854);
	// Useful infos
	int roadLenght = 10; // the lenght of the road. How many step the car will travel
	if (argc == 2)
	{
	    roadLenght = atoi(argv[1]);
	}
	int imageSize = 300; // the size of the square image wich will show the road
	int horizonPointOrdinate = 200; // the ordinate where the road will cross
	int horizonPointAbscissa = 150; // the abscissa where the road will cross
	int horizonOrdinate = 150; // the ordinate of the horizon (it is lower than the point where the road cross for estetics purposes)
	
	char pathStepImage [50]; // use to store the path of the image that visualize each step
	
	//////////////////////////
	//--- Initialisation ---//
	//////////////////////////
	
	// we create a car and initialize it
	Car carPosition;
	carPosition.abscissa = imageSize/2; // the car is centered at the beginning
	carPosition.directionAngle = M_PI/2; // and it goes straight forward by default
	
	// We create an empty road
	road* myRoad = createRoad(roadLenght, 0);
	// We atribute a different angle for each step of the road, so we know it goes left or right at each step
	generateStepsRoad(myRoad);
	// Create an image to see how the road looks like
	DonneesImageRGB* imageRoad = createImageRoad(myRoad);
	// We create the BMP image of the previsualisation of the road
	ecrisBMPRGB_Dans(imageRoad, "visualization/0 - road.bmp");
	
	/////////////////////////
	//--- step creation ---//
	/////////////////////////
	
	DonneesImageTab* stepTab = NULL; // Use to see the previsualization of each step. It is basicly what the driver see
	DonneesImageTab* stepTabGradiant = NULL; // Use to later find the point where the road go
	DonneesImageRGB* imageStep = NULL; // Use to store the previsualization that we will show
	Point* guessedCrossingPoint = NULL; // Will be use to store the point where the road cross that we detected (not the original one)
	// For each steps
	int i;
	for(i = 0; i < roadLenght; i++)
	{
	    // We show what step we currently are
	    printf("Step %d\n", i + 1);
	    // We calculate where the point will be, based on the road
	    horizonPointAbscissa = abscisseTrajectory(horizonPointOrdinate, myRoad->values[i], horizonOrdinate);
	    // We visualize where the road go
	    printf(" Road crossing abscissa : %d\n", horizonPointAbscissa);
	    stepTab = createRoadImage(horizonPointAbscissa, horizonPointOrdinate, carPosition, horizonOrdinate, 4, imageSize);
	    
	    // We reset the path of the step visualization image
	    strcpy(pathStepImage, "");
	    // We create the new one
	    sprintf(pathStepImage, "visualization/%d - step.bmp", i+1);
	    // We convert the visualization to a DonneesImageRGB to then create a .BMP image
	    imageStep = tabToRGB(stepTab);
	    ecrisBMPRGB_Dans(imageStep, pathStepImage);
	    
	    // We transform the image so it is grey
	    makeGreyLevel(stepTab);
	    // We create the gradiant image for further functions
	    stepTabGradiant = applyGradiantFilterOnTab(stepTab, PREWITT);
	    // We estimate where the road crosses
	    guessedCrossingPoint = getRoadPoint(stepTabGradiant);
	    
	    // We visualize what point we found
        printf(" Crossing abscissa found : %d\n", guessedCrossingPoint->x);
        // We shift the position of the car, relative to how much he shifted from the objective
        carPosition.abscissa = imageSize/2 + (guessedCrossingPoint->x - horizonPointAbscissa); // if their is no mistakes, the car stay centered
        // We visualize the new abscissa
        printf(" New car abscissa : %d\n", carPosition.abscissa);
        
        // We free everything for the next step
        libereDonneesTab(&stepTab); // the visualization of the step
        libereDonneesImageRGB(&imageStep); // the image of the step that we created
	    libereDonneesTab(&stepTabGradiant); // the gradiant image of it
	    free(guessedCrossingPoint); // and the point we guessed
	    guessedCrossingPoint = NULL;
	}
	
	/////////////////////////////
    //--- memory liberation ---//
    /////////////////////////////
    
	printf("Freeing the memory\n");
	
	freeRoad(&myRoad);
	libereDonneesImageRGB(&imageRoad);
	
	
	
	return 0;
}
