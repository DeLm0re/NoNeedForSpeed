#include "motif.h"

DonneesImageTab* localBinaryPattern(DonneesImageTab* tabImage)
{
	DonneesImageTab* newTabImage = initTab(tabImage->largeurImage, tabImage->hauteurImage);
	int i, j;
	int k, l;
	int cIndex;
	int filter[3][3];
	filter[0][0] = 1;
	filter[1][0] = 2;
	filter[2][0] = 4;
	filter[2][1] = 8;
	filter[2][2] = 16;
	filter[1][2] = 32;
	filter[0][2] = 64;
	filter[0][1] = 128;
	filter[1][1] = 0;
	int pixel;
	for(i = 0; i < tabImage->largeurImage; i++)
	{
		for(j = 0; j < tabImage->hauteurImage; j++)
		{
			for(cIndex = 0; cIndex < 3; cIndex++)
			{
				pixel = 0;
				for(k = 0; k < 3; k++)
				{
					for(l = 0; l < 3; l++)
					{
						if (0 <= i + k - 1 && i + k - 1 < tabImage->largeurImage &&
							0 <= j + l - 1 && j + l - 1 < tabImage->hauteurImage)
						{
							if (tabImage->donneesTab[i + k - 1][j + l - 1][cIndex] >= tabImage->donneesTab[i][j][cIndex])
							{
								pixel += filter[k][l];
							}
						}
					}
				}
				newTabImage->donneesTab[i][j][cIndex] = pixel;
			}
		}
	}
	return newTabImage;
}

float binaryPatternComp(DonneesImageTab* tabImage, DonneesImageTab* tabRef)
{
	float totalRelativeError = 0;
	Histogram* tabImageHistogram = createHistogram(tabImage, BLUE);
	Histogram* tabRefHistogram = createHistogram(tabRef, BLUE);
	int i;
	int iMax = min(tabImageHistogram->size, tabRefHistogram->size);
	for(i = 0; i < iMax; i++)
	{
		totalRelativeError += absValue(tabImageHistogram->values[i] - tabRefHistogram->values[i])/tabRefHistogram->values[i];
	}
	destructHistogram(&tabImageHistogram);
	destructHistogram(&tabRefHistogram);
	return totalRelativeError/iMax;
}
