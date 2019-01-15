#include "displayRoad.h"

/* @function
 *      Creates an image that shows the road from
 *      the point of view of the car
 * 
 * @param
 *      int horizonAbs  :   abscissa of the horizon point
 *      int horizonOrd  :   ordinate of the horizon point
 *      car carPosition :   position of the car
 *      int horizonHeight :   height of the horizonLine
 *      int roadWidth   :   width of the road in fraction of the imageSize (3 for 1/3 of the image)
 *      int imageSize   :   size of a side of the square image
 * 
 * @return  :   pointer on the DonneesImageTab created
 */
DonneesImageTab *createRoadImage(int horizonAbs, int horizonOrd, car carPosition, int horizonHeight, int roadWidth, int imageSize)
{
    DonneesImageTab *image = initTab(imageSize, imageSize);

    //Print the horizon point
    image->donneesTab[horizonAbs][horizonOrd][RED] = 255;

    //Print the horizon line
    printLine(image, 0, horizonHeight, imageSize-1, horizonHeight);

    //Print the road lines
    printLine(image, imageSize/roadWidth, 0, horizonAbs, horizonOrd);
    printLine(image, imageSize - (imageSize / roadWidth), 0, horizonAbs, horizonOrd);

    //Print the car
    image->donneesTab[carPosition.abscissa][0][RED] = 255;
    

    return image;
}

/* @function
 *      Print a line on a DonneesImageTab
 * 
 * @param
 *      DonneesImageTab *image  :   image to modify
 *      int x1  :   abscissa of the starting point
 *      int y1  :   ordinate of the starting point
 *      int x2  :   abscissa of the ending point
 *      int y2  :   ordinate of the ending point
 * 
 * @return  :   \
 */
void printLine(DonneesImageTab *image, int x1, int y1, int x2, int y2)
{
	int dx, dy;
	dx = x2 -x1;
	
	if(dx != 0) 
	{
		if(dx > 0) 
		{
			dy = y2 - y1;
			if(dy != 0) 
			{
				if(dy > 0) 
				{
					// vecteur oblique dans le 1er quadran
			  
					if(dx >= dy) 
					{
						// vecteur diagonal ou oblique proche de l’horizontale, dans le 1er octant
						int e = dx;
						dx = e * 2; // e est positif
						dy = dy * 2;  
						while(1) 
						{
							// déplacements horizontaux
							image->donneesTab[x1][y1][GREEN] = 255;
							x1 = x1 + 1;
							if(x1 == x2) 
							{
								break;
							}
							e = e - dy;
							if(e < 0) 
							{
								y1 = y1 + 1 ;  // déplacement diagonal
								e = e + dx ;
							}
						}
					}
					else 
					{
						// vecteur oblique proche de la verticale, dans le 2d octant
						int e = dy;
						dy = e * 2;  // e est positif
						dx = dx * 2;  
						while(1) 
						{  
							// déplacements verticaux
							image->donneesTab[x1][y1][GREEN] = 255;
							y1 = y1 + 1;
							if(y1 == y2) 
							{
								break;
							}
							e = e - dx;
							if(e < 0) 
							{
								x1 = x1 + 1;  // déplacement diagonal
								e = e + dy;
							}
						}
					}
				}
				else // dy < 0 (et dx > 0)  
				{
					// vecteur oblique dans le 4e cadran
			
					if (dx >= -dy) 
					{
						// vecteur diagonal ou oblique proche de l’horizontale, dans le 8e octant
						int e = dx;
						dx = e * 2; // e est positif
						dy = dy * 2;  
						while(1) 
						{  // déplacements horizontaux
							image->donneesTab[x1][y1][GREEN] = 255;
							x1 = x1 + 1;
							if(x1 == x2) 
							{
								break;
							}
							e = e + dy;
							if(e < 0) 
							{
								y1 = y1 - 1 ;  // déplacement diagonal
								e = e + dx ;
							}
						}
					}
					else 
					{  
						// vecteur oblique proche de la verticale, dans le 7e octant
						int e = dy;
						dy = e * 2; // e est négatif
						dx = dx * 2;  
						while(1) 
						{  
							// déplacements verticaux
							image->donneesTab[x1][y1][GREEN] = 255;
							y1 = y1 - 1;
							if(y1 == y2) 
							{
								break;
							}
							e = e + dx;
							if(e > 0) 
							{
								x1 = x1 + 1 ;  // déplacement diagonal
								e = e + dy ;
							}
						}
					}
				}
			}
			else // dy = 0 (et dx > 0) 
			{  
				// vecteur horizontal vers la droite
				do 
				{
					image->donneesTab[x1][y1][GREEN] = 255;
					x1 = x1 + 1;
				}while (x1 < x2);
			}
		}
		else // dx < 0
		{
			dy = y2 - y1;
			if(dy != 0) 
			{
				if(dy > 0) 
				{
					// vecteur oblique dans le 2d quadran
			  
					if (-dx >= dy) 
					{
						// vecteur diagonal ou oblique proche de l’horizontale, dans le 4e octant
						int e = dx;
						dx = e * 2; // e est négatif
						dy = dy * 2;  
						while(1) 
						{  // déplacements horizontaux
							image->donneesTab[x1][y1][GREEN] = 255;
							x1 = x1 - 1;
							if(x1 == x2) 
							{
								break;
							}
							e = e + dy;
							if(e >= 0) 
							{
								y1 = y1 + 1;  // déplacement diagonal
								e = e + dx ;
							}
						}
					}
					else 
					{
						// vecteur oblique proche de la verticale, dans le 3e octant
						int e = dy;
						dy = e * 2; // e est positif
						dx = dx * 2;  
						while(1) 
						{
							// déplacements verticaux
							image->donneesTab[x1][y1][GREEN] = 255;
							y1 = y1 + 1;
							if(y1 == y2) 
							{
								break;
							}
							e = e + dx;
							if(e <= 0) 
							{
								x1 = x1 - 1 ;  // déplacement diagonal
								e = e + dy ;
							}
						}
					}
				}  
				else // dy < 0 (et dx < 0)
				{
					// vecteur oblique dans le 3e cadran
					  
					if(dx <= dy) 
					{
						// vecteur diagonal ou oblique proche de l’horizontale, dans le 5e octant
						int e = dx;
						dx = e * 2; // e est négatif
						dy = dy * 2;  
						while(1) 
						{  
							// déplacements horizontaux
							image->donneesTab[x1][y1][GREEN] = 255;
							x1 = x1 - 1;
							if(x1 == x2) 
							{
								break;
							}
							e = e - dy;
							if(e >= 0) 
							{
								y1 = y1 - 1;  // déplacement diagonal
								e = e + dx;
							}
						}
					}
					else 
					{  
						// vecteur oblique proche de la verticale, dans le 6e octant
						int e = dy;
						dy = e * 2; // e est négatif
						dx = dx * 2;
						while(1) 
						{  
							// déplacements verticaux
							image->donneesTab[x1][y1][GREEN] = 255;
							y1 = y1 - 1;
							if(y1 == y2) 
							{
								break;
							}
							e = e -dx;
							if(e >= 0) 
							{
								x1 = x1 - 1;  // déplacement diagonal
								e = e + dy;
							}
						}
					}
					  
				}
			}
			else // dy = 0 (et dx < 0)
			{
				// vecteur horizontal vers la gauche
				do 
				{
					image->donneesTab[x1][y1][GREEN] = 255;
					x1 = x1 - 1;
				}while(x1 > x2);
			}
		}
	}
	else // dx = 0
	{
		dy = y2 - y1;
		if(dy != 0) 
		{
			if(dy > 0) 
			{
				// vecteur vertical croissant
				do 
				{
					image->donneesTab[x1][y1][GREEN] = 255;
					y1 = y1 + 1;
				}while(y1 < y2);
			}
			else 
			{
				// dy < 0 (et dx = 0)			
				// vecteur vertical décroissant
				do 
				{
					image->donneesTab[x1][y1][GREEN] = 255;
					y1 = y1 - 1;
				}while(y1 > y2);
			}
		}
	}
	image->donneesTab[x2][y2][GREEN] = 255;
 }