//General header files
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

//Other include
#include "image.h"

//Definition of the miniumum and the maximum angle a road can have (in radian)
#define ANGLE_ROAD_MIN (45 *M_PI/180)
#define ANGLE_ROAD_MAX (135 *M_PI/180)

//Definition of the road's image dimensions (must preferable to be a multiple of 2)
#define HEIGHT_IMAGE_ROAD (500)
#define WIDTH_IMAGE_ROAD (500)

//Definition of the road's lenght
#define LENGHT_ROAD (10)

//Définition of the type "road"
typedef struct road
{
    float *values;
    int lenght;
}road;

/**
* \fn road createRoad(int lenght, int value)
* \brief function that initialise and return a road
*
* \param lenght : the lenght of the road
* \param value : the value used to initialise each step of the road
* \return road* : a pointer on a road which is typedef in "road.h"
*/
road* createRoad(int lenght, float value);

/**
* \fn void freeRoad(road* myRoad)
* \brief function that free the memory allocated to a road
*
* \param road** : double pointer on the road we want to free
* \return void
*/
void freeRoad(road** myRoad);

/**
* \fn void generateStepsRoad(road* myRoad)
* \brief function that generate steps on a road
*
* \param road* : pointer on the road we want to modify to include steps
* \return void
*/
void generateStepsRoad(road* myRoad);

/**
* \fn void printfRoad(road* myRoad)
* \brief function that display the road in console
*
* \param road* : pointer on the road we want to display
* \return void
*/
void printfRoad(road* myRoad);

/**
* \fn DonneesImageRGB* createImageRoad(road* myRoad)
* \brief function that create an image of the road we can display with BmpLib
*
* \param road* : pointer on the road we want to display
* \return DonneesImageRGB* : pointer on an image DonneesImageRGB (image of the road which can be displayed)
*/
DonneesImageRGB* createImageRoad(road* myRoad);
