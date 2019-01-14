//Inclusion of the prototypes of the functions
#include "road.h"

/**
* \fn road createRoad(int lenght, int value)
* \brief function that initialise and return a road
*
* \param lenght : the lenght of the road
* \param value : the value used to initialise each step of the road
* \return road* : a pointer on a road which is typedef in "road.h"
*/
road* createRoad(int lenght, float value)
{
    int index;

    road *myRoad = NULL;

    myRoad = (road*)malloc(sizeof(road));

    myRoad->values = NULL;
    myRoad->lenght = lenght;

    myRoad->values = (float*)malloc(sizeof(float) * myRoad->lenght);

    for(index = 0; index < myRoad->lenght; index++)
    {
        myRoad->values[index] = value;
    }

    return(myRoad);
}

/**
* \fn void freeRoad(road* myRoad)
* \brief function that free the memory allocated to a road
*
* \param road** : double pointer on the road we want to free
* \return void
*/
void freeRoad(road** myRoad)
{
    free((*myRoad)->values);
    free(*myRoad);

    myRoad = NULL;
}

/**
* \fn void generateStepsRoad(road* myRoad)
* \brief function that generate steps on a road
*
* \param road* : pointer on the road we want to modify to include steps
* \return void
*/
void generateStepsRoad(road* myRoad)
{
    int index;

    for(index = 0; index < myRoad->lenght; index++)
    {
        myRoad->values[index] = ANGLE_ROAD_MIN + (float)(rand()) /( (float)(RAND_MAX/(ANGLE_ROAD_MAX-ANGLE_ROAD_MIN)));
    }
}

/**
* \fn void printfRoad(road* myRoad)
* \brief function that display the road in console
*
* \param road* : pointer on the road we want to display
* \return void
*/
void printfRoad(road* myRoad)
{
    int index;

    printf("\n\t ////   Value  of the road   //// \n");

    for(index = 0; index < myRoad->lenght; index++)
    {
        printf("\t //// %f | %f° //// \n", myRoad->values[index], myRoad->values[index]*180/M_PI);
    }

    printf("\t /////  ////  ////  ////   ///// \n\n");
}

/**
* \fn DonneesImageRGB* createImageRoad(road* myRoad)
* \brief function that create an image of the road we can display with BmpLib
*
* \param road* : pointer on the road we want to display
* \return DonneesImageRGB* : pointer on an image DonneesImageRGB (image of the road which can be displayed)
*/
DonneesImageRGB* createImageRoad(road* myRoad)
{
    int index;

    int height;
    int width;

    DonneesImageTab *myImageRoad = initTab(WIDTH_IMAGE_ROAD, HEIGHT_IMAGE_ROAD);

    int heightStep = (HEIGHT_IMAGE_ROAD-1)/LENGHT_ROAD;

    int currentHeight = 0;
    int currentWidth =  WIDTH_IMAGE_ROAD/2;

    float angleRadian;

    for(width = 0; width < WIDTH_IMAGE_ROAD; width++)
    {
        for(height = 0; height < HEIGHT_IMAGE_ROAD; height++)
        {
            myImageRoad->donneesTab[width][height][RED] = 255;
            myImageRoad->donneesTab[width][height][GREEN] = 255;
            myImageRoad->donneesTab[width][height][BLUE] = 255;
        }
    }

    myImageRoad->donneesTab[currentWidth][currentHeight][RED] = 0;
    myImageRoad->donneesTab[currentWidth][currentHeight][GREEN] = 0;
    myImageRoad->donneesTab[currentWidth][currentHeight][BLUE] = 0;

    for(index = 0; index < myRoad->lenght; index++)
    {
        angleRadian = myRoad->values[index];

        currentHeight = currentHeight + heightStep;

        if(angleRadian < 1.5708) //90°
        {
            currentWidth = (int) (currentWidth + (heightStep / tan(angleRadian)));
        }

        if(angleRadian > 1.5708) //90°
        {
            currentWidth = (int) (currentWidth - (heightStep / tan(M_PI - angleRadian)));
        }

        myImageRoad->donneesTab[currentWidth][currentHeight][RED] = 0;
        myImageRoad->donneesTab[currentWidth][currentHeight][GREEN] = 0;
        myImageRoad->donneesTab[currentWidth][currentHeight][BLUE] = 0;
    }

    return(tabToRGB(myImageRoad));
}
