#include "image.h"

float nmap(float value, float vmin, float vmax, float min, float max)
{
	// Original lenght
	float vlen = vmax - vmin;
	// Futur lenght
	float len = max - min;
	// Mapping the value
	float mapValue = (value-vmin)/vlen;
	return min + mapValue * len;
}

float gaussian(float x, float y, int filterSize)
{
	float teta;
	int offset;
	// If the filterSize is odd
	if (filterSize%2 == 1)
	{
		// We create a teta with the filterSize to be sure the gaussian funciton is wide enought
		teta = ((float) filterSize)/5;
		// We calculate the offset to center the gaussian graph
		offset = ((float) filterSize)/2;
	}
	else
	{
		// We create a teta with an odd filterSize to be sure the gaussian funciton is wide enought
		teta = ((float) filterSize + 1)/5;
		// We calculate the offset to center the gaussian graph
		offset = ((float) filterSize + 1)/2;
	}
	int i, j;
	// We calculate the factor by wich we will devide to normalize the guassian graph
	float normFactor = 0;
	for(i = -offset; i <= offset; i++)
	{
		for(j = -offset; j <= offset; j++)
		{
			normFactor += exp(-(i*i + j*j)/(2*teta*teta)) / (2*M_PI*teta*teta);
		}
	}
	// We return the value at the given coordinate
	return exp(-(x*x + y*y)/(2*teta*teta)) / (2*M_PI*teta*teta) / normFactor;
}

int pixelComp(void const *p1, void const *p2)
{
	char const cp1 = *((char*)p1);
	char const cp2 = *((char*)p2);
	if (cp1 > cp2) return 1;
	if (cp1 < cp2) return -1;
	return 0;
}

float max(float a, float b)
{
	if (a > b)
	{
		return a;
	}
	else
	{
		return b;
	}
}

int getMax(int* list, int size)
{
	qsort(list, size, sizeof(int), pixelComp);
	return list[size-1];
}

float min(float a, float b)
{
	if (a < b)
	{
		return a;
	}
	else
	{
		return b;
	}
}

int getMin(int* list, int size)
{
	qsort(list, size, sizeof(int), pixelComp);
	return list[0];
}

float absValue(float v)
{
	if (v < 0)
	{
		return -v;
	}
	else
	{
		return v;
	}
}

int sign(float v)
{
	return v/absValue(v);
}

DonneesImageRGB* initImage(int width, int height)
{
	int i;
	DonneesImageRGB* image = malloc(sizeof(DonneesImageRGB));
	image->largeurImage = width;
	image->hauteurImage = height;
	image->donneesRGB = malloc(sizeof(char) * width * height * 3);
	for(i = 0; i < width * height * 3; i++)
	{
		image->donneesRGB[i] = 0;
	}
	return image;
}

DonneesImageTab* initTab(int width, int height)
{
	int i, j, cIndex;
	DonneesImageTab* tabImage = malloc(sizeof(DonneesImageTab));
	tabImage->largeurImage = width;
	tabImage->hauteurImage = height;
	tabImage->donneesTab = malloc(sizeof(int**) * width);
	for(i = 0; i < width; i++)
	{
		tabImage->donneesTab[i] = malloc(sizeof(int*) * height);
		for(j = 0; j < height; j++)
		{
			tabImage->donneesTab[i][j] = malloc(sizeof(int) * 3);
			for(cIndex = 0; cIndex < 3; cIndex++)
			{
				tabImage->donneesTab[i][j][cIndex] = 0;
			}
		}
	}
	return tabImage;
}

Filter* initFilter(int width, int height)
{
	Filter* filter = malloc(sizeof(Filter));
	if (width%2 == 1)
	{
		filter->filterWidth = width;
	}
	else
	{
		filter->filterWidth = width + 1;
	}
	if (height%2 == 1)
	{
		filter->filterHeight = height;
	}
	else
	{
		filter->filterHeight = height + 1;
	}
	int i, j;
	filter->filterTab = malloc(sizeof(float*) * width);
	for(i = 0; i < width; i++)
	{
		filter->filterTab[i] = malloc(sizeof(float) * height);
		for(j = 0; j < height; j++)
		{
			filter->filterTab[i][j] = 0;
		}
	}
	return filter;
}

Line* initLine(int maxRIndex, int maxAngularIndex)
{
    Line* line = malloc(sizeof(Line));
	line->angularIndex = -1;
	line->rIndex = -1;
	line->maxRIndex = maxRIndex;
	line->maxAngularIndex = maxAngularIndex;
	line->startX = -1;
	line->startY = -1;
	line->endX = -1;
	line->endY = -1;
	line->lenght = 0;
	line->lenghtRatio = 0;
	return line;
}

Histogram* initHistogram(int size)
{
	Histogram* histogram = malloc(sizeof(Histogram));
	histogram->values = malloc(sizeof(int) * size);
	int i;
	for (i = 0; i < size; i++)
	{
		histogram->values[i] = 0;
	}
	histogram->size = size;
	return histogram;
}


void libereDonneesTab(DonneesImageTab** tabImage)
{
	if (tabImage != NULL)
	{
		if (*tabImage != NULL)
		{
			int i, j;
			for(i = 0; i < (*tabImage)->largeurImage; i++)
			{
				for(j = 0; j < (*tabImage)->hauteurImage; j++)
				{
					free((*tabImage)->donneesTab[i][j]);
				}
				free((*tabImage)->donneesTab[i]);
			}
			free((*tabImage)->donneesTab);
			free(*tabImage);
		}
		*tabImage = NULL;
	}
}

void destructFilter(Filter** filter)
{
	if (filter != NULL)
	{
		if (*filter != NULL)
		{
			int i;
			for(i = 0; i < (*filter)->filterWidth; i++)
			{
				free((*filter)->filterTab[i]);
			}
			free((*filter)->filterTab);
			free(*filter);
		}
		*filter = NULL;
	}
}

void destructHistogram(Histogram** histogram)
{
	if (histogram != NULL)
	{
		if (*histogram != NULL)
		{
			free((*histogram)->values);
			free(*histogram);
		}
		*histogram = NULL;
	}
}

DonneesImageTab* RGBToTab(DonneesImageRGB* image)
{
	int i, j;
	DonneesImageTab* tabImage = initTab(image->largeurImage, image->hauteurImage);
	for(i = 0; i < tabImage->largeurImage; i++)
	{
		tabImage->donneesTab[i] = malloc(sizeof(char*) * tabImage->hauteurImage);
		for(j = 0; j < tabImage->hauteurImage; j++)
		{
			tabImage->donneesTab[i][j] = malloc(sizeof(char) * 3);
			tabImage->donneesTab[i][j][BLUE] = image->donneesRGB[(image->largeurImage*j + i) * 3]; //blue
			tabImage->donneesTab[i][j][GREEN] = image->donneesRGB[(image->largeurImage*j + i) * 3 + 1]; //green
			tabImage->donneesTab[i][j][RED] = image->donneesRGB[(image->largeurImage*j + i) * 3 + 2]; //red
		}
	}
	return tabImage;
}

DonneesImageRGB* tabToRGB(DonneesImageTab* tabImage)
{
	DonneesImageRGB* image = initImage(tabImage->largeurImage, tabImage->hauteurImage);
	int i, j;
	int max = 0;
	for(i = 0; i < image->largeurImage; i++)
	{
		for(j = 0; j < image->hauteurImage; j++)
		{
			if (tabImage->donneesTab[i][j][BLUE] > max)
			{
				max = tabImage->donneesTab[i][j][BLUE];
			}
			if (tabImage->donneesTab[i][j][GREEN] > max)
			{
				max = tabImage->donneesTab[i][j][GREEN];
			}
			if (tabImage->donneesTab[i][j][RED] > max)
			{
				max = tabImage->donneesTab[i][j][RED];
			}
		}
	}
	int min = max;
	for(i = 0; i < image->largeurImage; i++)
	{
		for(j = 0; j < image->hauteurImage; j++)
		{
			if (tabImage->donneesTab[i][j][BLUE] < min)
			{
				min = tabImage->donneesTab[i][j][BLUE];
			}
			if (tabImage->donneesTab[i][j][GREEN] < min)
			{
				min = tabImage->donneesTab[i][j][GREEN];
			}
			if (tabImage->donneesTab[i][j][RED] < min)
			{
				min = tabImage->donneesTab[i][j][RED];
			}
		}
	}
	
	if (0 <= max && max < 255)
	{
		max = 255;
	}
	if (0 <= min && min < 255 && min < max)
	{
		min = 0;
	}
	
	for(i = 0; i < image->largeurImage; i++)
	{
		for(j = 0; j < image->hauteurImage; j++)
		{
			image->donneesRGB[(image->largeurImage*j + i) * 3] = 
				nmap(tabImage->donneesTab[i][j][BLUE], min, max, 0, 255);
			image->donneesRGB[(image->largeurImage*j + i) * 3 + 1] = 
				nmap(tabImage->donneesTab[i][j][GREEN], min, max, 0, 255);
			image->donneesRGB[(image->largeurImage*j + i) * 3 + 2] = 
				nmap(tabImage->donneesTab[i][j][RED], min, max, 0, 255);
		}
	}
	return image;
}

DonneesImageTab* cpyTab(DonneesImageTab* tabImage)
{
	int i, j;
	DonneesImageTab* newTabImage = initTab(tabImage->largeurImage, tabImage->hauteurImage);
	for(i = 0; i < newTabImage->largeurImage; i++)
	{
		newTabImage->donneesTab[i] = malloc(sizeof(char*) * newTabImage->hauteurImage);
		for(j = 0; j < newTabImage->hauteurImage; j++)
		{
			newTabImage->donneesTab[i][j] = malloc(sizeof(char) * 3);
			newTabImage->donneesTab[i][j][BLUE] = tabImage->donneesTab[i][j][BLUE];
			newTabImage->donneesTab[i][j][GREEN] = tabImage->donneesTab[i][j][GREEN];
			newTabImage->donneesTab[i][j][RED] = tabImage->donneesTab[i][j][RED];
		}
	}
	return newTabImage;
}

void makeGreyLevel(DonneesImageTab* tabImage)
{
	int i, j;
	float r, g, b;
	int grey;
	for(i = 0; i < tabImage->largeurImage; i++)
	{
		for(j = 0; j < tabImage->hauteurImage; j++)
		{
			// Little indian
			b = tabImage->donneesTab[i][j][BLUE];
			g = tabImage->donneesTab[i][j][GREEN];
			r = tabImage->donneesTab[i][j][RED];
			// We calculate the grey level using conventions
			grey = (int) floor(0.2126*r + 0.7152*g + 0.0722*b);
			tabImage->donneesTab[i][j][BLUE] = grey;
			tabImage->donneesTab[i][j][GREEN] = grey;
			tabImage->donneesTab[i][j][RED] = grey;
		}
	}
}

void cutBetweenLevel(DonneesImageTab* tabImage, int min, int max)
{
	int i, j;
	int colorNorm;
	int minNorm = sqrt(pow(min, 2)*3);
	int maxNorm = sqrt(pow(max, 2)*3);
	for (i = 0; i < tabImage->largeurImage; i++)
	{
		for (j = 0; j < tabImage->hauteurImage; j++)
		{
			colorNorm = sqrt(pow(tabImage->donneesTab[i][j][BLUE], 2) + 
				pow(tabImage->donneesTab[i][j][GREEN], 2) +
				pow(tabImage->donneesTab[i][j][RED], 2));
			if (colorNorm < minNorm)
			{
				tabImage->donneesTab[i][j][BLUE] = 0;
				tabImage->donneesTab[i][j][GREEN] = 0;
				tabImage->donneesTab[i][j][RED] = 0;
			}
			if (colorNorm > maxNorm)
			{
				tabImage->donneesTab[i][j][BLUE] = 255;
				tabImage->donneesTab[i][j][GREEN] = 255;
				tabImage->donneesTab[i][j][RED] = 255;
			}
				
		}
	}
}

Histogram* createHistogram(DonneesImageTab* tabImage, int color)
{
	int i, j;
	// We search for the maximum value
	int max = 0;
	for(i = 0; i < tabImage->largeurImage; i++)
	{
		for(j = 0; j < tabImage->hauteurImage; j++)
		{
			if (tabImage->donneesTab[i][j][color] > max)
			{
				max = tabImage->donneesTab[i][j][color];
			}
		}
	}
	if (max < 255)
	{
		max = 255;
	}
	// We initialize the histogram
	Histogram* histogram = initHistogram(max+1);
	// For each pixel of the image
	for(i = 0; i < tabImage->largeurImage; i++)
	{
		for(j = 0; j < tabImage->hauteurImage; j++)
		{
			if (tabImage->donneesTab[i][j][color] < max+1)
			{
				// We count up the correct column of the histogram
				histogram->values[tabImage->donneesTab[i][j][color]]++;
			}
		}
	}
	return histogram;
}

DonneesImageRGB* histogramToRGB(Histogram* histogram)
{
	// We init the image which will contain the histogram
	DonneesImageRGB* image = initImage(histogram->size, 256);
	int i, j;
	// We find the maximum value of the histogram
	int max = 0;
	for(i = 0; i < histogram->size; i++)
	{
		if (histogram->values[i] > max)
		{
			max = histogram->values[i];
		}
	}
	// for each row
	for(j = 0; j < image->largeurImage; j++)
	{
		// In each column
		for(i = 0; i < image->hauteurImage; i++)
		{
			// We light the pixel in blue if his index is lower than the corresponding histogram column
			//We use a maped version of the value in the histogram using the maximum value we found previously
			if (i <= nmap(histogram->values[j], 0, max, 0, 255))
			{
				image->donneesRGB[(image->largeurImage*i + j) * 3] = 200;
				image->donneesRGB[(image->largeurImage*i + j) * 3 + 1] = 50;
				image->donneesRGB[(image->largeurImage*i + j) * 3 + 2] = 50;
			}
			// Otherwise, we light the pixel in white
			else
			{
				image->donneesRGB[(image->largeurImage*i + j) * 3] = 255;
				image->donneesRGB[(image->largeurImage*i + j) * 3 + 1] = 255;
				image->donneesRGB[(image->largeurImage*i + j) * 3 + 2] = 255;
			}
		}
	}
	return image;
}

DonneesImageTab* createHough(DonneesImageTab* tabImage, int sensibility, int nbrAngularSteps)
{
	int i, j, cIndex;
	int r, a;
	// We calculate the maximum r value possible (distance to the origine)
	float maxR = sqrt(pow(tabImage->largeurImage, 2) + pow(tabImage->hauteurImage, 2));
	float angle;
	// We init the tab that will contain the Hough transform. We use maxR*2 to save the negative values
	DonneesImageTab* tabHough = initTab(nbrAngularSteps, maxR*2);
	
	// For each pixel on our image
	for(i = 0; i < tabImage->largeurImage; i++)
	{
		for(j = 0; j < tabImage->hauteurImage; j++)
		{
			// And for each color
			for (cIndex = 0; cIndex < 3; cIndex++)
			{
				// If the pixel is concidered usefull (superior to the given sensibility)
				if (tabImage->donneesTab[i][j][cIndex] > sensibility)
				{
					// We check all the lines that go throught this point
					//So for each angle until we did all the measurement
					for (a = 0; a < nbrAngularSteps; a++)
					{
						// We map the angle of th eline between 0 and pi so it's in radiant
						//(0 to pi and pi to 2pi have the same information, so we only check the first half)
						angle = nmap(a, 0, nbrAngularSteps-1, 0, M_PI);
						// We calculate the distance to the center of the line
						r = (int) i * cos(angle) + j * sin(angle) + maxR;
						// We increase the corresponding valur in our Hough table
						tabHough->donneesTab[a][r][cIndex]++;
					}
				}
				
			}
		}
	}
	return tabHough;
}

DonneesImageRGB* houghToRGB(DonneesImageTab* tabHough)
{
	// We initialize the image we will return
	DonneesImageRGB* image = initImage(tabHough->largeurImage, tabHough->largeurImage);
	int i, j, cIndex;
	int k, l;
	// We get the maximum value in the Hough table
	int max = 0;
	for(i = 0; i < tabHough->largeurImage; i++)
	{
		for(j = 0; j < tabHough->hauteurImage; j++)
		{
			for(cIndex = 0; cIndex < 3; cIndex++)
			{
				if (tabHough->donneesTab[i][j][cIndex] > max)
				{
					max = tabHough->donneesTab[i][j][cIndex];
				}
			}
		}
	}
	// For each pixel of the image
	for(i = 0; i < image->largeurImage; i++)
	{
		for(j = 0; j < image->hauteurImage; j++)
		{
			// We map the coordinate of the image to the coordinate of our Hough table
			k = (int) nmap(i, 0, image->largeurImage-1, 0, tabHough->largeurImage-1);
			l = (int) nmap(j, 0, image->hauteurImage-1, 0, tabHough->hauteurImage-1);
			// We save the value of our Hough transforme in the image by maping the value between 0 and 255
			image->donneesRGB[(image->largeurImage*j + i) * 3] = 
				(char) nmap(tabHough->donneesTab[k][l][BLUE], 0, max, 0, 255);
			image->donneesRGB[(image->largeurImage*j + i) * 3 + 1] = 
				(char) nmap(tabHough->donneesTab[k][l][GREEN], 0, max, 0, 255);
			image->donneesRGB[(image->largeurImage*j + i) * 3 + 2] = 
				(char) nmap(tabHough->donneesTab[k][l][RED], 0, max, 0, 255);
		}
	}
	return image;
}

Line* getMaxLine(DonneesImageTab* tabHough)
{
	int i, j, cIndex;
	Line* maxLine = initLine(tabHough->hauteurImage, tabHough->largeurImage);
	int max = 0;
	// For each pixel in the Hough matrice
	for(i = 0; i < tabHough->largeurImage; i++)
	{
		for(j = 0; j < tabHough->hauteurImage; j++)
		{
			for(cIndex = 0; cIndex < 3; cIndex++)
			{
				if (tabHough->donneesTab[i][j][cIndex] > max)
				{
					max = tabHough->donneesTab[i][j][cIndex];
					maxLine->angularIndex = i;
					maxLine->rIndex = j;
				}
			}
		}
	}
	return maxLine;
}

void updateLineInfo(DonneesImageTab* tabImage, Line* line, int sensibility)
{
	float maxR = sqrt(pow(tabImage->largeurImage, 2) + pow(tabImage->largeurImage, 2));
	float angle = nmap(line->angularIndex, 0, line->maxAngularIndex-1, 0, M_PI);
	float radius = nmap(line->rIndex, 0, line->maxRIndex-1, -maxR, maxR);
	float m, n;
	int i, j;
	int pixels = 0;
	// Mostly horizontal line
	if(M_PI/4 <= angle && angle <= 3*M_PI/4)
	{
		m = tan(angle - M_PI/2);
		n = radius / sin(angle);
		for(i = 0; i < tabImage->largeurImage; i++)
		{
			j = m*i+n;
			if (0 <= j && j < tabImage->hauteurImage &&
				tabImage->donneesTab[i][j][BLUE] > sensibility && line->startX == -1)
			{
				line->startX = i;
			}
			if (0 <= j && j < tabImage->hauteurImage &&
				tabImage->donneesTab[i][j][BLUE] > sensibility)
			{
				pixels++;
			}
			
			j = m*(tabImage->largeurImage-i-1)+n;
			if (0 <= j && j < tabImage->hauteurImage &&
				tabImage->donneesTab[tabImage->largeurImage-i-1][j][BLUE] > sensibility && line->endX == -1)
			{
				line->endX = tabImage->largeurImage-i-1;
			}
		}
		line->startY = m*line->startX+n;
		line->endY = m*line->endX+n;
	}
	// Mostly vertical line
	else
	{
		m = -tan(angle);
		n = radius / cos(angle);
		for(j = 0; j < tabImage->hauteurImage; j++)
		{
			i = m*j+n;
			if (0 <= i && i < tabImage->largeurImage &&
				tabImage->donneesTab[i][j][BLUE] > sensibility && line->startY == -1)
			{
				line->startY = j;
			}
			if (0 <= i && i < tabImage->largeurImage &&
				tabImage->donneesTab[i][j][BLUE] > sensibility)
			{
				pixels++;
			}
			
			i = m*(tabImage->hauteurImage-j-1)+n;
			if (0 <= i && i < tabImage->largeurImage &&
				tabImage->donneesTab[i][tabImage->hauteurImage-j-1][BLUE] > sensibility && line->endY == -1)
			{
				line->endY = tabImage->hauteurImage-j-1;
			}
		}
		line->startX = m*line->startY+n;
		line->endX = m*line->endY+n;
	}
	float len = sqrt(pow(line->endX-line->startX, 2) + pow(line->endY - line->startY, 2));
	line->lenght = len;
	line->lenghtRatio = pixels/len;
}

void traceLineOnImage(DonneesImageTab* tabImage, Line* line, int r, int g, int b)
{
	float maxR = sqrt(pow(tabImage->largeurImage, 2) + pow(tabImage->largeurImage, 2));
	float angle = nmap(line->angularIndex, 0, line->maxAngularIndex-1, 0, M_PI);
	float radius = nmap(line->rIndex, 0, line->maxRIndex-1, -maxR, maxR);
	float m, n;
	int i, j;
	// Mostly horizontal lines
	if(M_PI/4 <= angle && angle <= 3*M_PI/4)
	{
		m = tan(angle - M_PI/2);
		n = radius / sin(angle);
		for(i = line->startX; i < line->endX; i += sign(line->endX - line->startX))
		{
			j = m*i+n;
			if (0 <= j && j < tabImage->hauteurImage)
			{
				tabImage->donneesTab[i][j][BLUE] = b;
				tabImage->donneesTab[i][j][GREEN] = g;
				tabImage->donneesTab[i][j][RED] = r;
			}
		}
	}
	// Mostly vertical lines
	else
	{
		m = -tan(angle);
		n = radius / cos(angle);
		for(j = line->startY; j < line->endY; j += sign(line->endY - line->startY))
		{
			i = m*j+n;
			if (0 <= i && i < tabImage->largeurImage)
			{
				tabImage->donneesTab[i][j][BLUE] = b;
				tabImage->donneesTab[i][j][GREEN] = g;
				tabImage->donneesTab[i][j][RED] = r;
			}
		}
	}
}

Filter* createAvarageFilter(int width, int height)
{
	Filter* filter = initFilter(width, height);
	int i, j;
	for(i = 0; i < width; i++)
	{
		for(j = 0; j < height; j++)
		{
			filter->filterTab[i][j] = 1 / ((float)(width * height));
		}
	}
	return filter;
}

Filter* createGaussianFilter(int width, int height)
{
	Filter* filter = initFilter(width, height);
	int i, j;
	int xOffset = (int) filter->filterWidth/2;
	int yOffset = (int) filter->filterHeight/2;
	for(i = 0; i < width; i++)
	{
		for(j = 0; j < height; j++)
		{
			filter->filterTab[i][j] = gaussian(i - xOffset, j - yOffset, width);
		}
	}
	return filter;
}

Filter* createLaplacianFilter(int type)
{
	Filter* filter = initFilter(3, 3);
	int i, j;
	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 3; j++)
		{
			filter->filterTab[i][j] = -1;
		}
	}
	switch (type)
	{
		case FOUR_CX:
			filter->filterTab[1][1] = 4;
			filter->filterTab[2][2] = 0;
			filter->filterTab[2][0] = 0;
			filter->filterTab[0][2] = 0;
			filter->filterTab[0][0] = 0;
			break;
		case HEIGHT_CX:
			filter->filterTab[1][1] = 8;
			break;
	}
	return filter;
}

DonneesImageTab* applyFilterOnTab(DonneesImageTab* tabImage, Filter* filter)
{
	DonneesImageTab* newTabImage = cpyTab(tabImage);
	int i, j;
	int k, l;
	int cIndex;
	float pixel;
	int kOffset;
	int lOffset;
	for(i = 0; i < tabImage->largeurImage; i++)
	{
		for(j = 0; j < tabImage->hauteurImage; j++)
		{
			pixel = 0;
			kOffset = (int) filter->filterWidth/2;
			lOffset = (int) filter->filterHeight/2;
			for(cIndex = 0; cIndex < 3; cIndex++)
			{
				pixel = 0;
				for(k = -kOffset; k <= kOffset; k++)
				{
					for(l = -lOffset; l <= lOffset; l++)
					{
						if(0 <= i + k && i + k < tabImage->largeurImage &&
							0 <= j + l && j + l < tabImage->hauteurImage)
						{
							pixel += filter->filterTab[k + kOffset][l + lOffset] *
								tabImage->donneesTab[i + k][j + l][cIndex];
						}
					}
				}
				if(pixel > 255)
				{
					pixel = 255;
				}
				if(pixel < 0)
				{
					pixel = 0;
				}
				newTabImage->donneesTab[i][j][cIndex] = (char) pixel;
			}
		}
	}
	return newTabImage;
}

DonneesImageTab* applyMedianFilterOnTab(DonneesImageTab* tabImage, const int filterWidth, const int filterHeight)
{
	DonneesImageTab* newTabImage = cpyTab(tabImage);
	int i, j;
	int k, l;
	int cIndex;
	int kOffset = (int) filterWidth/2;
	int lOffset = (int) filterHeight/2;
	int deadPixel;
	int filter[filterWidth*filterHeight];
	int pixel;
	for(i = 0; i < tabImage->largeurImage; i++)
	{
		for(j = 0; j < tabImage->hauteurImage; j++)
		{
			for(cIndex = 0; cIndex < 3; cIndex++)
			{
				deadPixel = 0;
				for(k = -kOffset; k <= kOffset; k++)
				{
					for(l = -lOffset; l <= lOffset; l++)
					{
						if(0 <= i + k && i + k < tabImage->largeurImage &&
							0 <= j + l && j + l < tabImage->hauteurImage)
						{
							filter[(l + lOffset) + (k + kOffset)*filterWidth - deadPixel] =
								tabImage->donneesTab[i + k][j + l][cIndex];
						}
						else
						{
							deadPixel++;
						}
					}
				}
				qsort(filter, filterWidth*filterHeight - deadPixel, sizeof(int), pixelComp);
				pixel = filter[(int) floor((filterWidth*filterHeight - deadPixel)/2)];
				if (pixel < 0)
				{
					pixel = 0;
				}
				if (pixel > 255)
				{
					pixel = 255;
				}
				newTabImage->donneesTab[i][j][cIndex] = pixel;
			}
		}
	}
	return newTabImage;
}

DonneesImageTab* applyRobertsFilterOnTab(DonneesImageTab* tabImage)
{
	DonneesImageTab* newTabImage = cpyTab(tabImage);
	int i, j;
	int cIndex;
	int pixelX, pixelY;
	int pixel;
	for(i = 0; i < tabImage->largeurImage; i++)
	{
		for(j = 0; j < tabImage->hauteurImage; j++)
		{
			for(cIndex = 0; cIndex < 3; cIndex++)
			{
				pixelX = 0;
				pixelY = 0;
				if(i + 1 < tabImage->largeurImage && j + 1 < tabImage->hauteurImage)
				{
					pixelX = tabImage->donneesTab[i][j + 1][cIndex] - tabImage->donneesTab[i][j][cIndex];
					pixelY = tabImage->donneesTab[i + 1][j][cIndex] - tabImage->donneesTab[i][j][cIndex];
				}
				pixel = sqrt(pixelX*pixelX + pixelY*pixelY);
				if (pixel < 0)
				{
					pixel = 0;
				}
				if (pixel > 255)
				{
					pixel = 255;
				}
				newTabImage->donneesTab[i][j][cIndex] = pixel;
			}
		}
	}
	return newTabImage;
}

DonneesImageTab* applyGradiantFilterOnTab(DonneesImageTab* tabImage, int type)
{
	DonneesImageTab* newTabImage = cpyTab(tabImage);
	int i, j;
	int k, l;
	int cIndex;
	int xFilter[3][3];
	xFilter[0][0] = xFilter[0][2] = -1;
	xFilter[1][0] = xFilter[1][1] = xFilter[1][2] = 0;
	xFilter[2][0] = xFilter[2][2] = 1;
	xFilter[0][1] = -type;
	xFilter[2][1] = type;
	int yFilter[3][3];
	yFilter[0][0] = yFilter[2][0] = -1;
	yFilter[0][1] = yFilter[1][1] = yFilter[2][1] = 0;
	yFilter[0][2] = yFilter[2][2] = 1;
	yFilter[1][0] = -type;
	yFilter[1][2] = type;
	int pixelX, pixelY;
	int pixel;
	for(i = 0; i < tabImage->largeurImage; i++)
	{
		for(j = 0; j < tabImage->hauteurImage; j++)
		{
			for(cIndex = 0; cIndex < 3; cIndex++)
			{
				pixelX = 0;
				pixelY = 0;
				if(1 <= i && i < tabImage->largeurImage-1 &&
					1 <= j && j < tabImage->hauteurImage-1)
				{
					for(k = -1; k <= 1; k++)
					{
						for(l = -1; l <= 1; l++)
						{
							pixelX += xFilter[k + 1][l + 1] * tabImage->donneesTab[i + k][j + l][cIndex];
							pixelY += yFilter[k + 1][l + 1] * tabImage->donneesTab[i + k][j + l][cIndex];
						}
					}
					pixel = sqrt(pixelX*pixelX + pixelY*pixelY);
				}
				else
				{
					pixel = 0;
				}
				if (pixel < 0)
				{
					pixel = 0;
				}
				if (pixel > 255)
				{
					pixel = 255;
				}
				newTabImage->donneesTab[i][j][cIndex] = pixel;
			}
		}
	}
	return newTabImage;
}

void applyDillatationFilter(DonneesImageTab* tabImage, int whiteLevel)
{
    int i, j, cIndex;
    for(i = 0; i < tabImage->largeurImage; i++)
	{
		for(j = 0; j < tabImage->hauteurImage; j++)
		{
		    for(cIndex = 0; cIndex < 3; cIndex++)
		    {
		        if (areNeighboursWhite(tabImage, whiteLevel, i, j))
		        {
		            tabImage->donneesTab[i][j][cIndex] = -1;
		        }
		    }
		}
	}
	for(i = 0; i < tabImage->largeurImage; i++)
	{
		for(j = 0; j < tabImage->hauteurImage; j++)
		{
		    for(cIndex = 0; cIndex < 3; cIndex++)
		    {
		        if (tabImage->donneesTab[i][j][cIndex] == -1)
		        {
		            tabImage->donneesTab[i][j][cIndex] = 255;
		        }
		    }
		}
	}
}

bool areNeighboursWhite(DonneesImageTab* tabImage, int whiteLevel, int x, int y)
{
    int i, j, cIndex;
    bool isWhite = false;
    for(i = -1; i <= 1; i++)
	{
		for(j = -1; j <= 1; j++)
		{
		    for(cIndex = 0; cIndex < 3; cIndex++)
		    {
		        if (0 <= x + i && x + i < tabImage->largeurImage && 
		            0 <= y + j && y + j < tabImage->hauteurImage &&
		            tabImage->donneesTab[x + i][y + j][cIndex] >= whiteLevel)
		        {
		            isWhite = true;
		        }
		    }
		}
	}
	return isWhite;
}

void applyErosionFilter(DonneesImageTab* tabImage, int blackLevel)
{
    int i, j, cIndex;
    for(i = 0; i < tabImage->largeurImage; i++)
	{
		for(j = 0; j < tabImage->hauteurImage; j++)
		{
		    for(cIndex = 0; cIndex < 3; cIndex++)
		    {
		        if (areNeighboursBlack(tabImage, blackLevel, i, j))
		        {
		            tabImage->donneesTab[i][j][cIndex] = -1;
		        }
		    }
		}
	}
	for(i = 0; i < tabImage->largeurImage; i++)
	{
		for(j = 0; j < tabImage->hauteurImage; j++)
		{
		    for(cIndex = 0; cIndex < 3; cIndex++)
		    {
		        if (tabImage->donneesTab[i][j][cIndex] == -1)
		        {
		            tabImage->donneesTab[i][j][cIndex] = 0;
		        }
		    }
		}
	}
}

bool areNeighboursBlack(DonneesImageTab* tabImage, int blackLevel, int x, int y)
{
    int i, j, cIndex;
    bool isBlack = false;
    for(i = -1; i <= 1; i++)
	{
		for(j = -1; j <= 1; j++)
		{
		    for(cIndex = 0; cIndex < 3; cIndex++)
		    {
		        if (0 <= x + i && x + i < tabImage->largeurImage && 
		            0 <= y + j && y + j < tabImage->hauteurImage &&
		            tabImage->donneesTab[x + i][y + j][cIndex] >= 0 &&
		            tabImage->donneesTab[x + i][y + j][cIndex] <= blackLevel)
		        {
		            isBlack = true;
		        }
		    }
		}
	}
	return isBlack;
}

DonneesImageTab* applyLocalBinaryPattern(DonneesImageTab* tabImage)
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
