//Inclusion of the prototypes of the functions
#include "road.h"

/**
* \fn road createRoad(int lengh, int value)
* \brief function that initialise and return a road
*
* \param lengh : the lengh of the road
* \param value : the value used to initialise each step of the road
* \return road : a road which is typedef in "road.h"
*/
road createRoad(int lengh, int value)
{
    int index;

    road myRoad;

    myRoad = (road)malloc(sizeof(int) * lengh);

    for(index = 0; index < lengh; index++)
    {
        myRoad[index] = value;
    }

    return(myRoad);
}

/**
* \fn void freeRoad(road* myRoad)
* \brief function that free the memory allocated to a road
*
* \param road* : pointer on the road we want to free
* \return void
*/
void freeRoad(road* myRoad)
{
    free(*myRoad);
    myRoad = NULL;
}
