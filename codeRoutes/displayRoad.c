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
DonneesImageTab *createRoadImage(int horizonAbs, int horizonOrd, Car carPosition, int horizonHeight, int roadWidth, int imageSize)
{
    //Image of the road
    DonneesImageTab *image = initTab(imageSize, imageSize);

    //Index used to browse the image
    int index;

    //Print the ground
    for(index = horizonHeight - 1; index >= 0; index--)
    {
        printLine(image, 0, index, imageSize-1, index, 10, 128, 10);
    }

    //Print the road
    for(index = imageSize/2 - roadWidth/2; index < imageSize/2 + roadWidth/2; index++)
    {
        printLine(image, horizonAbs, horizonOrd, index, 0, 64, 64, 64);
    }

    //Print the road left line
    for(index = imageSize/2 - roadWidth/2; index < imageSize/2 - roadWidth/2 + (roadWidth/20); index++)
    {
        printLine(image, horizonAbs, horizonOrd, index, 0, 192, 192, 192);
    }
    
    //Print the road right line
    for(index = imageSize/2 + roadWidth/2; index > imageSize/2 + roadWidth/2 - (roadWidth/20); index--)
    {
        printLine(image, horizonAbs, horizonOrd, index, 0, 192, 192, 192);
    }

    //Print the horizon line
    printLine(image, 0, horizonHeight, imageSize-1, horizonHeight, 0, 128, 255);
    //And the sky
    for(index = horizonHeight + 1; index < image->hauteurImage; index++)
    {
        printLine(image, 0, index, imageSize-1, index, 0, 128, 255);
    }

    //Print the horizon point
    image->donneesTab[horizonAbs][horizonOrd][RED] = 255;

    //Print the car
    printSprite("sprites/car.bmp", image, carPosition.abscissa - (SPRITE_WIDTH / 2), 0);
    
    return image;
}



/* @function
 *      Print a sprite on an image
 * 
 * @param
 *      char *spriteName    :   path and name of the sprite
 *      DonneesImageTab *image  :   image to modify
 *      int spriteAbs   :   abscissa of the sprite (bottom left point)
 *      int spriteOrd   :   ordinate of the sprite (bottom left point)
 * 
 * @return  :   \
 */
void printSprite(char *spriteName, DonneesImageTab *image, int spriteAbs, int spriteOrd)
{
    //Get the sprite tab
    DonneesImageRGB *bmpSprite = lisBMPRGB(spriteName);
    DonneesImageTab *sprite = RGBToTab(bmpSprite);

    //Browse the sprite
    int absIndex, ordIndex;
    for(absIndex = 0; absIndex < sprite->largeurImage; absIndex++)
    {
        for(ordIndex = 0; ordIndex < sprite->hauteurImage; ordIndex++)
        {
            //Coordinates of the pixel in the image
            int realAbs = absIndex + spriteAbs;
            int realOrd = ordIndex + spriteOrd;

            //Check if in the image
            if(realAbs >= 0 && realAbs < image->largeurImage 
                && realOrd >= 0 && realOrd < image->hauteurImage )
            {
                int spriteR = sprite->donneesTab[absIndex][ordIndex][RED];
                int spriteG = sprite->donneesTab[absIndex][ordIndex][GREEN];
                int spriteB = sprite->donneesTab[absIndex][ordIndex][BLUE];

                //Check if not background color
                if(spriteR != SPRITE_BACKGROUND_R || spriteG != SPRITE_BACKGROUND_G 
                    || spriteB != SPRITE_BACKGROUND_B)
                {
                    image->donneesTab[realAbs][realOrd][RED] = spriteR;
                    image->donneesTab[realAbs][realOrd][GREEN] = spriteG;
                    image->donneesTab[realAbs][realOrd][BLUE] = spriteB;
                }
            }
        }
    }
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
 *      int red :   red value of the color of the line
 *      int green:  green value of the color of the line
 *      int blue:   blue value of the color of the line 
 * 
 * @return  :   \
 */
void printLine(DonneesImageTab *image, int x1, int y1, int x2, int y2, int red, int green, int blue)
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
                            image->donneesTab[x1][y1][RED] = red;
							image->donneesTab[x1][y1][GREEN] = green;
                            image->donneesTab[x1][y1][BLUE] = blue;
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
                            image->donneesTab[x1][y1][RED] = red;
							image->donneesTab[x1][y1][GREEN] = green;
                            image->donneesTab[x1][y1][BLUE] = blue;
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
                            image->donneesTab[x1][y1][RED] = red;
							image->donneesTab[x1][y1][GREEN] = green;
                            image->donneesTab[x1][y1][BLUE] = blue;
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
                            image->donneesTab[x1][y1][RED] = red;
							image->donneesTab[x1][y1][GREEN] = green;
                            image->donneesTab[x1][y1][BLUE] = blue;
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
                    image->donneesTab[x1][y1][RED] = red;
                    image->donneesTab[x1][y1][GREEN] = green;
                    image->donneesTab[x1][y1][BLUE] = blue;
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
                            image->donneesTab[x1][y1][RED] = red;
							image->donneesTab[x1][y1][GREEN] = green;
                            image->donneesTab[x1][y1][BLUE] = blue;
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
                            image->donneesTab[x1][y1][RED] = red;
							image->donneesTab[x1][y1][GREEN] = green;
                            image->donneesTab[x1][y1][BLUE] = blue;
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
                            image->donneesTab[x1][y1][RED] = red;
							image->donneesTab[x1][y1][GREEN] = green;
                            image->donneesTab[x1][y1][BLUE] = blue;
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
                            image->donneesTab[x1][y1][RED] = red;
							image->donneesTab[x1][y1][GREEN] = green;
                            image->donneesTab[x1][y1][BLUE] = blue;
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
                    image->donneesTab[x1][y1][RED] = red;
                    image->donneesTab[x1][y1][GREEN] = green;
                    image->donneesTab[x1][y1][BLUE] = blue;
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
                    image->donneesTab[x1][y1][RED] = red;
                    image->donneesTab[x1][y1][GREEN] = green;
                    image->donneesTab[x1][y1][BLUE] = blue;
					y1 = y1 + 1;
				}while(y1 < y2);
			}
			else 
			{
				// dy < 0 (et dx = 0)			
				// vecteur vertical décroissant
				do 
				{
                    image->donneesTab[x1][y1][RED] = red;
                    image->donneesTab[x1][y1][GREEN] = green;
                    image->donneesTab[x1][y1][BLUE] = blue;
					y1 = y1 - 1;
				}while(y1 > y2);
			}
		}
	}
    image->donneesTab[x1][y1][RED] = red;
    image->donneesTab[x1][y1][GREEN] = green;
    image->donneesTab[x1][y1][BLUE] = blue;
 }
