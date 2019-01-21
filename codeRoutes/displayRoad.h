#include "roadDetection.h"

typedef struct Car
{
    int abscissa;
    float directionAngle;
}Car;

/* @function
 *      Creates an image that shows the road from
 *      the point of view of the car
 *
 * @param
 *      int horizonAbs  :   abscissa of the horizon point
 *      int horizonOrd  :   ordinate of the horizon point
 *      car carPosition :   position of the car
 *      int horizonHeight :   height of the horizonLine
 *      int roadWidth   :   width of the road
 *      int imageSize   :   size of a side of the square image
 *
 * @return  :   pointer on the DonneesImageTab created
 */
DonneesImageTab *createRoadImage(int horizonAbs, int horizonOrd, Car carPosition, int horizonHeight, int roadWidth, int imageSize);


/* @function
 *      Print a line on a DonneesImageTab
 *
 * @param
 *      DonneesImageTab *image  :   image to modify
 *      int x1  :   abscissa of the starting point
 *      int y1  :   ordinate of the starting point
 *      int x2  :   abscissa of the ending point
 *      int y2  :   ordinate of the ending point
 *      int red :   red value of the color of the line
 *      int green:  green value of the color of the line
 *      int blue:   blue value of the color of the line
 *
 * @return  :   \
 */
void printLine(DonneesImageTab *image, int x1, int y1, int x2, int y2, int red, int green, int blue);
