
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

    ////////////////////////////
	//--- Global variables ---//
	////////////////////////////
	
	int roadLenght = 10; // the lenght of the road. How many step the car will travel
	// If their is an argument
	if (argc == 2)
	{
	    // Then we set the roadLenght acordingly
	    roadLenght = atoi(argv[1]);
	}
	
	const int imageSize = 300; // the size of the square image wich will show the road (what the driver will see)
	const int horizonPointOrdinate = 200; // the ordinate where the road will cross
	int horizonPointAbscissa = 150; // the abscissa where the road will cross
	const int horizonOrdinate = 150; // the ordinate of the horizon (it is lower than the point where the road cross for estetics purposes)
	const int roadWidth = 200; // the width of the road that we will visualize
	const float angularStep = M_PI/32; // the amount we will shift the angle direction of the car each time we fail to find a correct one
	//  Currently 5.625° or a sixteenth of 90°
	
	char pathStepImage [50]; // use to store the path of the image that visualize each step
	
	//////////////////////////
	//--- Initialisation ---//
	//////////////////////////
	
	// Initializing the random with a seed
	srand(10495854);
	
	// We erase all previously detected signs
    int errorSystem = system("rm -f visualization/*");
    if (errorSystem)
    {
        printf("Error : couldn't remove files in the following folder : panneaux\n");
    }
	
	// we create a car and initialize it
	Car carPosition;
	carPosition.abscissa = imageSize/2; // the car is centered at the beginning
	carPosition.directionAngle = M_PI/2; // and it goes straight forward by default
	
	// We create an empty road
	road* myRoad = createRoad(roadLenght, 0);
	// We atribute a different angle for each step of the road, so we know how much it goes left or right at each step
	generateStepsRoad(myRoad);
	// Create an image to see how the road looks like
	DonneesImageRGB* imageRoad = createImageRoad(myRoad);
	// We write the BMP image of the previsualisation of the road
	ecrisBMPRGB_Dans(imageRoad, "visualization/0 - road.bmp");
	
	/////////////////////////
	//--- step creation ---//
	/////////////////////////
	
	DonneesImageTab* stepTab = NULL; // Use to see the previsualization of each step. It is basicly what the driver see
	DonneesImageRGB* imageStep = NULL; // Use to create the BMP image of the previsualization
	float lastWorkingDirection[5] = // Use to store the last working direction the car went. Used for the Q-learning
	    {M_PI/2, -1, -1, -1, -1}; // The first direction is concider to be straight forward for code simplification purposes
    int dirIndex = 0; // use to check the last choosen direction. Will be use to navigate in "lastWorkingDirection"
    bool isDecisionValid = false; // use to know if the direction we shoosed is correct (if the car doesn't go of the road)
    
	// For each steps (each segment of the road in "myRoad")
	int i;
	for(i = 0; i < roadLenght; i++)
	{
	    //--- creation of the visualization of the road
	    
	    // We show what step we currently are
	    printf("Step %d\n", i + 1);
	    // We calculate where the road will go on the vizaulisation, based on the values in "myRoad"
	    // This abscissa is also the best abscissa for the car to go, because it is centered on the road
	    horizonPointAbscissa = abscisseTrajectory(horizonPointOrdinate, myRoad->values[i], horizonOrdinate);
	    // We print the abscissa we found
	    printf(" Road crossing abscissa : %d\n", horizonPointAbscissa);
	    printf(" Current car abscissa : %d\n", carPosition.abscissa);
	    // We visualize where the road go
	    stepTab = createRoadImage(horizonPointAbscissa, horizonPointOrdinate, carPosition, horizonOrdinate, roadWidth, imageSize);
	    
	    //--- creation of an image to show the visualization
	    
	    // We reset the path of the step visualization image
	    strcpy(pathStepImage, "");
	    // We create the new one
	    sprintf(pathStepImage, "visualization/%d - step.bmp", i+1);
	    // We convert the visualization to a DonneesImageRGB to then create a .BMP image
	    imageStep = tabToRGB(stepTab);
	    ecrisBMPRGB_Dans(imageStep, pathStepImage);
	    
	    //--- Finding the direction the car will go
	    
	    // We reset the flag which indicate if the angle direction of the car is valid or not
	    isDecisionValid = false;
	    // To get where the car must go
	    // First we check the last 5 working angle direction
	    for (dirIndex = 0; dirIndex < 5; dirIndex++)
	    {
	        // Their could be less than 5 working direction, so we check that their is an existing one
	        // We also check if we hadn't already find a working direction
	        if (lastWorkingDirection[dirIndex] != -1 && !isDecisionValid)
	        {
	            // Once we found a previous direction, we store it into the car structure
	            // (this direction is the angle we will test)
	            carPosition.directionAngle = lastWorkingDirection[dirIndex];
	            // Then, we check if this direction is valid or not
	            // If it is valid, the flag "isDecisionValid" will be set to true, otherwise it will be set to false
	            isDecisionValid = isOnTheRoad(
	                abscisseTrajectory(horizonPointOrdinate, carPosition.directionAngle, carPosition.abscissa), // The abscissa the car will go next
	                roadWidth, // The width of the road
	                horizonPointAbscissa); // The center of the road at the next step
	        }
	    }

	    // If we didn't find a valid direction with the 5 previous one, we try new, more random ones
	    dirIndex = 1; // We also reset the dirIndex because we will use it to check further and further angles
	    while(!isDecisionValid) // while we didn't find a valid direction
	    {
	        // We shift the angle the car will go by a little amount from the direction it had at the last step
	        //This little formula will shift the angle from left to right, further and further at each loop
	        //  ((dirIndex%2)*2-1) is 1 or -1 depending on the loop
	        //  floor(dirIndex/2) is the magnitud of the shift. it will grow by one every two loops, starting at 1
	        //  angularStep is the amount of shifting we do
	        carPosition.directionAngle = lastWorkingDirection[0] + ((dirIndex%2)*2-1)*floor(dirIndex/2)*angularStep;
	        // Then we check if the new direction is valid or not
	        isDecisionValid = isOnTheRoad(
	                abscisseTrajectory(horizonPointOrdinate, carPosition.directionAngle, carPosition.abscissa), 
	                roadWidth, 
	                horizonPointAbscissa);
	        dirIndex++;
	    }
	    
	    // Once we found a valid direction, we save the new abscissa of the car
	    carPosition.abscissa = 
	        imageSize/2 - 
	        (abscisseTrajectory(horizonPointOrdinate, carPosition.directionAngle, carPosition.abscissa) - horizonPointOrdinate);
	    // then, we save the angular direction into "lastWorkingDirection" by shifting every one of them by one
	    for (dirIndex = 4; dirIndex > 0; dirIndex--)
	    {
	        lastWorkingDirection[dirIndex] = lastWorkingDirection[dirIndex-1];
	    }
	    lastWorkingDirection[0] = carPosition.directionAngle;
        
        // We free everything for the next step
        libereDonneesTab(&stepTab); // the visualization of the step
        libereDonneesImageRGB(&imageStep); // the image of the step that we created
	}
	
	/////////////////////////////
    //--- memory liberation ---//
    /////////////////////////////
    
	printf("Freeing the memory\n");
	
	freeRoad(&myRoad);
	libereDonneesImageRGB(&imageRoad);
	
	
	
	return 0;
}
