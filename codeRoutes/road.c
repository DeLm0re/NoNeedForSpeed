//Inclusion of the prototypes of the functions
#include "routes.h"

/**
 * \fn Field initialiseField(int height, int width)
 * \brief function that initialise our field to make our environment
 *
 * \param height : height of the field, must be 1 widen than the original size because the edges is initialized with -1
 * \param width : width of the field, must be 1 widen than the original size because the edges is initialized with -1
 * \return Field : A field, which is a tydef declared in core.h (2D array)
 */
 road* createRoad(int lengh, int value)
 {
     int index;

     road *myRoad;

     myRoad = (road*)malloc(sizeof(int) * lengh);

     for(index = 0; index < lengh; index++)
     {
         myRoad[index] = value;
     }

     return(myRoad);
 }
