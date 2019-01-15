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

int main(void)
{
	srand((unsigned)(time(0)));

	road *myRoad = NULL;

	myRoad = createRoad(LENGTH_ROAD, 0);

	generateStepsRoad(myRoad);

	printRoad(myRoad);

	ecrisBMPRGB_Dans(createImageRoad(myRoad), "roadSteps.bmp");

	freeRoad(&myRoad);

	return 0;
}
