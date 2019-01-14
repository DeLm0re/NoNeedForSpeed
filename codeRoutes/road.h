//General header files
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

//Other include
#include "image.h"

//Definition of the miniumum and the maximum angle a road can have (in radian)
#define ANGLE_ROAD_MIN (0.785398) //45°
#define ANGLE_ROAD_MAX (2.35619) //135°

//Définition of the type "road"
typedef struct road
{
    float *values;
    int lengh;
}road;

/**
* \fn road createRoad(int lengh, int value)
* \brief function that initialise and return a road
*
* \param lengh : the lengh of the road
* \param value : the value used to initialise each step of the road
* \return road* : a pointoer on a road which is typedef in "road.h"
*/
road* createRoad(int lengh, float value);

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
