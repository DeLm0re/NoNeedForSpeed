//Inclusion of the prototypes of the functions
#include "road.h"

/**
* \fn road createRoad(int lengh, int value)
* \brief function that initialise and return a road
*
* \param lengh : the lengh of the road
* \param value : the value used to initialise each step of the road
* \return road* : a pointoer on a road which is typedef in "road.h"
*/
road* createRoad(int lengh, float value)
{
    int index;

    road *myRoad = NULL;

    myRoad = (road*)malloc(sizeof(road));

    myRoad->values = NULL;
    myRoad->lengh = lengh;

    myRoad->values = (float*)malloc(sizeof(float) * myRoad->lengh);

    for(index = 0; index < myRoad->lengh; index++)
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

    for(index = 0; index < myRoad->lengh; index++)
    {
        myRoad->values[index] = ANGLE_ROAD_MIN + (float)(rand()) /( (float)(RAND_MAX/(ANGLE_ROAD_MAX-ANGLE_ROAD_MIN)));
    }
}

void printfRoad(road* myRoad)
{
    int index;

    printf("\n\t ////   Value  of the road   //// \n");

    for(index = 0; index < myRoad->lengh; index++)
    {
        printf("\t //// %f | %f° //// \n", myRoad->values[index], myRoad->values[index]*180/PI);
    }

    printf("\t /////  ////  ////  ////   ///// \n\n");
}
