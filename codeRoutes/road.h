//General header files
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//Définition of the type "road" (dynamic allocation)
typedef int *road;

/**
* \fn road createRoad(int lengh, int value)
* \brief function that initialise and return a road
*
* \param lengh : the lengh of the road
* \param value : the value used to initialise each step of the road
* \return road : a road which is typedef in "road.h"
*/
road createRoad(int lengh, int value);

/**
* \fn void freeRoad(road* myRoad)
* \brief function that free the memory allocated to a road
*
* \param road* : pointer on the road we want to free
* \return void
*/
void freeRoad(road* myRoad);
