#include "region.h"

DonneesImageTab* initTabRegion(int width, int height)
{
	// We initialize a DonneesImageTab using the initTab function
	DonneesImageTab* tabRegion = initTab(width, height);
	// Then we put all the value in it at UNCHECKED (-2)
	int i, j, cIndex;
	for(i = 0; i < width; i++)
	{
		for(j = 0; j < height; j++)
		{
			for(cIndex = 0; cIndex < 3; cIndex++)
			{
				tabRegion->donneesTab[i][j][cIndex] = UNCHECKED;
			}
		}
	}
	// finaly we return it
	return tabRegion;
}

IdRegion* initIdRegion(int blue, int green, int red, int x, int y, int label)
{
	IdRegion* idRegion = malloc(sizeof(IdRegion));
	idRegion->blue = blue;
	idRegion->green = green;
	idRegion->red = red;
	idRegion->x = x;
	idRegion->y = y;
	idRegion->label = label;
	return idRegion;
}

IdRegions* initIdRegions(int size)
{
	IdRegions* idRegions = malloc(sizeof(IdRegions));
	idRegions->size = size;
	idRegions->regions = malloc(sizeof(IdRegion*)*size);
	int i;
	for (i = 0; i < size; i++)
	{
		idRegions->regions[i] = NULL;
	}
	return idRegions;
}

void destructIdRegions(IdRegions** idRegions)
{
	if (idRegions != NULL)
	{
		if (*idRegions != NULL)
		{
			int i;
			for (i = 0; i < (*idRegions)->size; i++)
			{
				free((*idRegions)->regions[i]);
			}
			free((*idRegions)->regions);
			free(*idRegions);

			*idRegions = NULL;
		}
	}
}

IdRegion* findRegionBottomUp(DonneesImageTab* tabImage, DonneesImageTab* tabRegion, int x, int y, int label, int sensibility)
{
	IdRegion* idRegion = NULL;
	// If the seed is in the image
	if (0 <= x && x < tabRegion->largeurImage &&
		0 <= y && y < tabRegion->hauteurImage)
	{
		// We save the red, blue and green value of the seed
		idRegion = initIdRegion(
			tabImage->donneesTab[x][y][BLUE],
			tabImage->donneesTab[x][y][GREEN],
			tabImage->donneesTab[x][y][RED],
			x,
			y,
			label);
		int tempColor; // Use to calculate the distance (in colors) between a given pixel and the seed
		// We set the pixel at the seed's coordinate  in the region matrice to BORDER (-1) to say that it needs to be check
		tabRegion->donneesTab[x][y][BLUE] = BORDER;
		tabRegion->donneesTab[x][y][GREEN] = BORDER;
		tabRegion->donneesTab[x][y][RED] = BORDER;
		int borderFind; // Use to know if their is still pixels to be check (BORDER close enought to the seed color)
		int i, j;
		int k, l;
		do
		{
			borderFind = 0;
			// For each pixels
			for(i = 0; i < tabRegion->largeurImage; i++)
			{
				for(j = 0; j < tabRegion->hauteurImage; j++)
				{
					// If it needs to be check
					if (tabRegion->donneesTab[i][j][BLUE] == BORDER)
					{
						// We calculate the distance (in colors) between the current pixel and the seed
						tempColor = sqrt(
							pow(tabImage->donneesTab[i][j][BLUE] - idRegion->blue, 2) +
							pow(tabImage->donneesTab[i][j][GREEN] - idRegion->green, 2) +
							pow(tabImage->donneesTab[i][j][RED] - idRegion->red, 2));
						// If it is the a color close enought (distance inferior to the sensibility)
						if(tempColor < sensibility)
						{
							// We add it to the region matrice
							tabRegion->donneesTab[i][j][BLUE] = idRegion->label;
							tabRegion->donneesTab[i][j][GREEN] = idRegion->label;
							tabRegion->donneesTab[i][j][RED] = idRegion->label;
							// We say that we found a new pixel to add to the region
							borderFind++;
							// Then, for each neighbours
							for(k = -1; k <= 1; k++)
							{
								for(l = -1; l <= 1; l++)
								{
									// If they have not been checked yet and if they are in the bounderies
									if (0 <= i + k && i + k < tabRegion->largeurImage &&
										0 <= j + l && j + l < tabRegion->hauteurImage &&
										tabRegion->donneesTab[i + k][j + l][BLUE] == UNCHECKED &&
										tabRegion->donneesTab[i + k][j + l][GREEN] == UNCHECKED &&
										tabRegion->donneesTab[i + k][j + l][RED] == UNCHECKED)
									{
										// Say that it's a border to check later (set it a BORDER value)
										tabRegion->donneesTab[i + k][j + l][BLUE] = BORDER;
										tabRegion->donneesTab[i + k][j + l][GREEN] = BORDER;
										tabRegion->donneesTab[i + k][j + l][RED] = BORDER;
									}
								}
							}
						}
					}
				}
			}
		// We ontinue until their is no point left to check
		}while (borderFind > 0);

		// For each pixels and colors, we set the remaining pixel with a BORDER value to an UNCHECKED value
		//(used if we need to add other region on this matrice)
		int cIndex;
		for(i = 0; i < tabRegion->largeurImage; i++)
		{
			for(j = 0; j < tabRegion->hauteurImage; j++)
			{
				for(cIndex = 0; cIndex < 3; cIndex++)
				{
					if (tabRegion->donneesTab[i][j][cIndex] == BORDER)
					{
						tabRegion->donneesTab[i][j][cIndex] = UNCHECKED;
					}
				}
			}
		}
	}
	return idRegion;
}

IdRegions* findAllRegionBottomUp(DonneesImageTab* tabImage, DonneesImageTab* tabRegion, int sensibility)
{
	// Use to store all IdRegion
	IdRegions* idRegions = initIdRegions(1);
	// Use to store the idRegions while realocating memories
	IdRegions* tempIdRegions = NULL;
	// Use to store the IdRegion fo the region found
	IdRegion* tempIdRegion = NULL;
	int i, j;
	int k;
	int label = 0;
	// Use to know the number of region found
	int nbrRegion = 0;

	// For each pixels
	for (i = 0; i < tabRegion->largeurImage; i++)
	{
		for (j = 0; j < tabRegion->hauteurImage; j++)
		{
			// If it is empty (valid starting point)
			if (tabRegion->donneesTab[i][j][BLUE] == UNCHECKED &&
				tabRegion->donneesTab[i][j][GREEN] == UNCHECKED &&
				tabRegion->donneesTab[i][j][RED] == UNCHECKED)
			{
				// We find the region which contain the starting point
				tempIdRegion = findRegionBottomUp(tabImage, tabRegion, i, j, label, sensibility);
				// We change the label
				label+=10;
				// We say that we find one more region
				nbrRegion++;

				// If we can save it in idRegions
				if (idRegions->size >= nbrRegion)
				{
					// We put it in the list of idRegion
					idRegions->regions[nbrRegion - 1] = tempIdRegion;
				}
				// If not
				else
				{
					// We realoc idRegions
					// First by initializing an IdRegions which will be use to transfer the value
					tempIdRegions = initIdRegions(nbrRegion);
					// We copy everything into the previously initialized IdRegion
					for(k = 0; k < idRegions->size; k++)
					{
						tempIdRegions->regions[k] = idRegions->regions[k];
						idRegions->regions[k] = NULL;
					}
					// We reInitialize idRegions
					destructIdRegions(&idRegions);
					idRegions = initIdRegions(nbrRegion);
					// Then, we put back all the value
					for(k = 0; k < idRegions->size; k++)
					{
						idRegions->regions[k] = tempIdRegions->regions[k];
						tempIdRegions->regions[k] = NULL;
					}
					// And we destruct the temporary IdRegions
					destructIdRegions(&tempIdRegions);

					// Finaly, we add the found region
					idRegions->regions[nbrRegion - 1] = tempIdRegion;
				}
			}
		}
	}
	// And we return the list of idRegion
	return idRegions;
}

IdRegions* findAllColorRegionBottomUp(DonneesImageTab* tabImage, DonneesImageTab* tabRegion, int red, int green, int blue, int sensibility)
{
	// Use to store all IdRegion
	IdRegions* idRegions = initIdRegions(1);
	// Use to store the idRegions while realocating memories
	IdRegions* tempIdRegions = NULL;
	// Use to store the IdRegion fo the region found
	IdRegion* tempIdRegion = NULL;
	int i, j;
	int k;
	int label = 0;
	// Use to know the number of region found
	int nbrRegion = 0;
	
	// For each pixels
	for (i = 0; i < tabRegion->largeurImage; i++)
	{
		for (j = 0; j < tabRegion->hauteurImage; j++)
		{
			// If it is empty (valid starting point)
			if (tabRegion->donneesTab[i][j][BLUE] == UNCHECKED &&
				tabRegion->donneesTab[i][j][GREEN] == UNCHECKED &&
				tabRegion->donneesTab[i][j][RED] == UNCHECKED &&
				floor(tabImage->donneesTab[i][j][BLUE] - blue) <= 15 &&
				floor(tabImage->donneesTab[i][j][GREEN] - green) <= 15 &&
				floor(tabImage->donneesTab[i][j][RED] - red) <= 15)
			{
				// We find the region which contain the starting point
				tempIdRegion = findRegionBottomUp(tabImage, tabRegion, i, j, label, sensibility);
				// We change the label
				label+=10;
				// We say that we find one more region
				nbrRegion++;
				
				// If we can save it in idRegions
				if (idRegions->size >= nbrRegion)
				{
					// We put it in the list of idRegion
					idRegions->regions[nbrRegion - 1] = tempIdRegion;
				}
				// If not
				else
				{
					// We realoc idRegions
					// First by initializing an IdRegions which will be use to transfer the value
					tempIdRegions = initIdRegions(nbrRegion);
					// We copy everything into the previously initialized IdRegion
					for(k = 0; k < idRegions->size; k++)
					{
						tempIdRegions->regions[k] = idRegions->regions[k];
						idRegions->regions[k] = NULL;
					}
					// We reInitialize idRegions
					destructIdRegions(&idRegions);
					idRegions = initIdRegions(nbrRegion);
					// Then, we put back all the value
					for(k = 0; k < idRegions->size; k++)
					{
						idRegions->regions[k] = tempIdRegions->regions[k];
						tempIdRegions->regions[k] = NULL;
					}
					// And we destruct the temporary IdRegions
					destructIdRegions(&tempIdRegions);
					
					// Finaly, we add the found region
					idRegions->regions[nbrRegion - 1] = tempIdRegion;
				}
			}
		}
	}
	// And we return the list of idRegion
	return idRegions;
}

IdRegion* whatIsNeighboorsColor(DonneesImageTab* tabRegion, int x, int y)
{
	int i, j;
	IdRegion* idRegion = NULL;
	for (i = -1; i <= 1; i++)
	{
		for (j = -1; j <= 1; j++)
		{
			if (0 <= x + i && x + i < tabRegion->largeurImage &&
				0 <= y + j && y + j < tabRegion->hauteurImage &&
				tabRegion->donneesTab[x + i][y + j][BLUE] != UNCHECKED &&
				tabRegion->donneesTab[x + i][y + j][GREEN] != UNCHECKED &&
				tabRegion->donneesTab[x + i][y + j][RED] != UNCHECKED &&
				idRegion == NULL)
			{
				idRegion = malloc(sizeof(IdRegion));
				idRegion->blue = tabRegion->donneesTab[x + i][y + j][BLUE];
				idRegion->green = tabRegion->donneesTab[x + i][y + j][GREEN];
				idRegion->red = tabRegion->donneesTab[x + i][y + j][RED];
				idRegion->x = x + i;
				idRegion->y = y + j;
			}
		}
	}
	return idRegion;
}

DonneesImageTab* getShape(DonneesImageTab* tabRegion,  IdRegion* idRegion)
{
	int i, j;
	DonneesImageTab* tabShape = NULL;
	bool isInRegion = false;;
	// Getting the height of the region
	int startY = tabRegion->hauteurImage; // the maximum Y coordinate wich contain the shape
	int endY = -1; // the minimum Y coordinate wich contain the shape
	for (i = 0; i < tabRegion->largeurImage; i++)
	{
		isInRegion = false;
		for (j = 0; j < tabRegion->hauteurImage; j++)
		{
			// If we are in the region
			if (tabRegion->donneesTab[i][j][BLUE] == idRegion->label &&
				tabRegion->donneesTab[i][j][GREEN] == idRegion->label &&
				tabRegion->donneesTab[i][j][RED] == idRegion->label)
			{
				// We activate the corresponding flag
				isInRegion = true;
				// And we update the startY only if it is lower than the previous one
				if (startY > j)
				{
					startY = j;
				}
			}
			// If we are not in the region and if we where previously
			if (tabRegion->donneesTab[i][j][BLUE] != idRegion->label &&
				tabRegion->donneesTab[i][j][GREEN] != idRegion->label &&
				tabRegion->donneesTab[i][j][RED] != idRegion->label &&
				isInRegion)
			{
				// We deactivate the corresponding flag
				isInRegion = false;
				// And we update the endY only if it is higher than the previous one
				if (endY < j)
				{
					endY = j;
				}
			}
		}
	}

	// Getting the width of the region
	int startX = tabRegion->largeurImage;
	int endX = -1;
	for (j = 0; j < tabRegion->hauteurImage; j++)
	{
		isInRegion = false;
		for (i = 0; i < tabRegion->largeurImage; i++)
		{
			// If we are in the region
			if (tabRegion->donneesTab[i][j][BLUE] == idRegion->label &&
				tabRegion->donneesTab[i][j][GREEN] == idRegion->label &&
				tabRegion->donneesTab[i][j][RED] == idRegion->label)
			{
				// We activate the corresponding flag
				isInRegion = true;
				// And we update the startX only if it is lower than the previous one
				if (startX > i)
				{
					startX = i;
				}
			}
			// If we are not in the region and if we where previously
			if (tabRegion->donneesTab[i][j][BLUE] != idRegion->label &&
				tabRegion->donneesTab[i][j][GREEN] != idRegion->label &&
				tabRegion->donneesTab[i][j][RED] != idRegion->label &&
				isInRegion)
			{
				// We deactivate the corresponding flag
				isInRegion = false;
				// And we update the endX only if it is higher than the previous one
				if (endX < i)
				{
					endX = i;
				}
			}
		}
	}

	// If we have found the width and the height of the region
	if (endX >= 0 && endY >= 0 && startX >= 0 && startY >= 0)
	{
		// We initialise the DonnesImageTab with the corect lenght
		tabShape = initTab(endX - startX, endY-startY);
		// And we fill the region in
		for (i = 0; i < tabShape->largeurImage; i++)
		{
			for (j = 0; j < tabShape->hauteurImage; j++)
			{
				if (tabRegion->donneesTab[i + startX][j + startY][BLUE] == idRegion->label &&
					tabRegion->donneesTab[i + startX][j + startY][GREEN] == idRegion->label &&
					tabRegion->donneesTab[i + startX][j + startY][RED] == idRegion->label)
				{
					tabShape->donneesTab[i][j][BLUE] = idRegion->blue;
					tabShape->donneesTab[i][j][GREEN] = idRegion->green;
					tabShape->donneesTab[i][j][RED] = idRegion->red;
				}
				else
				{
					tabShape->donneesTab[i][j][BLUE] = UNCHECKED;
					tabShape->donneesTab[i][j][GREEN] = UNCHECKED;
					tabShape->donneesTab[i][j][RED] = UNCHECKED;
				}
			}
		}

	}
	return tabShape;
}

DonneesImageTab* getOriginalShape(DonneesImageTab* tabImage, DonneesImageTab* tabRegion,  IdRegion* idRegion)
{
	int i, j;
	DonneesImageTab* tabShape = NULL;
	bool isInRegion = false;;
	// Getting the height of the region
	int startY = tabRegion->hauteurImage; // the maximum Y coordinate wich contain the shape
	int endY = -1; // the minimum Y coordinate wich contain the shape
	for (i = 0; i < tabRegion->largeurImage; i++)
	{
		isInRegion = false;
		for (j = 0; j < tabRegion->hauteurImage; j++)
		{
			// If we are in the region
			if (tabRegion->donneesTab[i][j][BLUE] == idRegion->label &&
				tabRegion->donneesTab[i][j][GREEN] == idRegion->label &&
				tabRegion->donneesTab[i][j][RED] == idRegion->label)
			{
				// We activate the corresponding flag
				isInRegion = true;
				// And we update the startY only if it is lower than the previous one
				if (startY > j)
				{
					startY = j;
				}
			}
			// If we are not in the region and if we where previously
			if (tabRegion->donneesTab[i][j][BLUE] != idRegion->label &&
				tabRegion->donneesTab[i][j][GREEN] != idRegion->label &&
				tabRegion->donneesTab[i][j][RED] != idRegion->label &&
				isInRegion)
			{
				// We deactivate the corresponding flag
				isInRegion = false;
				// And we update the endY only if it is higher than the previous one
				if (endY < j)
				{
					endY = j;
				}
			}
		}
	}

	// Getting the width of the region
	int startX = tabRegion->largeurImage;
	int endX = -1;
	for (j = 0; j < tabRegion->hauteurImage; j++)
	{
		isInRegion = false;
		for (i = 0; i < tabRegion->largeurImage; i++)
		{
			// If we are in the region
			if (tabRegion->donneesTab[i][j][BLUE] == idRegion->label &&
				tabRegion->donneesTab[i][j][GREEN] == idRegion->label &&
				tabRegion->donneesTab[i][j][RED] == idRegion->label)
			{
				// We activate the corresponding flag
				isInRegion = true;
				// And we update the startX only if it is lower than the previous one
				if (startX > i)
				{
					startX = i;
				}
			}
			// If we are not in the region and if we where previously
			if (tabRegion->donneesTab[i][j][BLUE] != idRegion->label &&
				tabRegion->donneesTab[i][j][GREEN] != idRegion->label &&
				tabRegion->donneesTab[i][j][RED] != idRegion->label &&
				isInRegion)
			{
				// We deactivate the corresponding flag
				isInRegion = false;
				// And we update the endX only if it is higher than the previous one
				if (endX < i)
				{
					endX = i;
				}
			}
		}
	}

	// If we have found the width and the height of the region
	if (endX >= 0 && endY >= 0 && startX >= 0 && startY >= 0)
	{
		// We initialise the DonnesImageTab with the corect lenght
		tabShape = initTab(endX - startX, endY-startY);
		// And we fill the region in
		for (i = 0; i < tabShape->largeurImage; i++)
		{
			for (j = 0; j < tabShape->hauteurImage; j++)
			{
				tabShape->donneesTab[i][j][BLUE] = tabImage->donneesTab[i + startX][j + startY][BLUE];
				tabShape->donneesTab[i][j][GREEN] = tabImage->donneesTab[i + startX][j + startY][GREEN];
				tabShape->donneesTab[i][j][RED] = tabImage->donneesTab[i + startX][j + startY][RED];
			}
		}

	}
	return tabShape;
}

void regionToBinary(DonneesImageTab* tabImage)
{
    int i, j;
    for(i = 0; i < tabImage->largeurImage; i++)
    {
        for(j = 0; j < tabImage->hauteurImage; j++)
        {
            if (tabImage->donneesTab[i][j][BLUE] == UNCHECKED)
            {
                tabImage->donneesTab[i][j][BLUE] = 255;
                tabImage->donneesTab[i][j][GREEN] = 255;
                tabImage->donneesTab[i][j][RED] = 255;
            }
            else
            {
                tabImage->donneesTab[i][j][BLUE] = 0;
                tabImage->donneesTab[i][j][GREEN] = 0;
                tabImage->donneesTab[i][j][RED] = 0;
            }
        } 
    }
}

void writeAllRegion(DonneesImageTab *imageRegion, IdRegions *allRegions)
{
	int index, i, j;
	char bufferName[50];
	char totalBuffer[50];
	DonneesImageTab* currentRegion = NULL;
	DonneesImageRGB* currentRegionRGB = NULL;

    //system("rm -f regions/*");
	for(index = 0; index < allRegions->size; index++)
	{
		sprintf(bufferName, "%d", index);
		strcat(bufferName, "_e.bmp");
		strcpy(totalBuffer, "./regions/");
		strcat(totalBuffer, bufferName);
		
        currentRegion = getShape(imageRegion, allRegions->regions[index]);
        
        if (currentRegion != NULL)
        {
            for (i = 0; i < currentRegion->largeurImage; i++)
            {  
                for (j = 0; j < currentRegion->hauteurImage; j++)
                {
                    if (currentRegion->donneesTab[i][j][BLUE] == UNCHECKED)
                    {
                        currentRegion->donneesTab[i][j][BLUE] = 255;
                        currentRegion->donneesTab[i][j][GREEN] = 255;
                        currentRegion->donneesTab[i][j][RED] = 255;
                    }
                    else
                    {
                        currentRegion->donneesTab[i][j][BLUE] = 0;
                        currentRegion->donneesTab[i][j][GREEN] = 0;
                        currentRegion->donneesTab[i][j][RED] = 0;
                    }
                }
            }
            currentRegionRGB = tabToRGB(currentRegion);
		    ecrisBMPRGB_Dans( currentRegionRGB, totalBuffer);
		    
		    libereDonneesTab(&currentRegion);
		    libereDonneesImageRGB(&currentRegionRGB);
		}
		

		strcpy(bufferName, "");
		strcpy(totalBuffer, "");
	}
}
