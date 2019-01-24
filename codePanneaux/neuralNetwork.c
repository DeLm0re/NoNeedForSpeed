#include "neuralNetwork.h"


/* @function
 *      Creates a neurone with default values
 *      and weights initialized randomly
 * 
 * @param
 *      int weightAbsDimention  :   dimention of the weight matrix in abscissa
 *      int weightOrdDimention  :   dimention of the weight matrix in ordinate
 *      Letter targetLetter     :   letter that this neurone can recognize
 * 
 * @return  :   neurone created
 * */
Neurone *createNeurone(int weightAbsDimention, int weigthOrdDimention, Letter targetLetter)
{
    int absIndex, ordIndex;

    //Memory allocation
	Neurone *neurone = (Neurone*)malloc(sizeof(Neurone));

    neurone->targetLetter = targetLetter;

    //Dimentions of the weight matrix of the neurone
    neurone->weightAbs = weightAbsDimention;
    neurone->weightOrd = weigthOrdDimention;

    //Weight matrix allocation
    neurone->weights = (int**)malloc(weightAbsDimention * sizeof(int*));

    for(absIndex = 0; absIndex < weightAbsDimention; absIndex++)
    {
        neurone->weights[absIndex] = (int*)malloc(weigthOrdDimention * sizeof(int));
    }
	
	//Weights equels 0 at the beginning
	for(absIndex = 0; absIndex < weightAbsDimention; absIndex++)
    {
        for(ordIndex = 0; ordIndex < weigthOrdDimention; ordIndex++)
        {
            neurone->weights[absIndex][ordIndex] = 0;
        }
    }
	
	return neurone;
}


/* @function
 *      Calculate the weighted sum of an image
 *      for a given neurone
 * 
 * @param
 *      DonneesImageTab *binaryImage    :   image to analyze
 *      Neurone *workingNeurone     :   neurone analyzing
 * 
 * @return  :   result of the sum
 */
int calculateWeightedSum(DonneesImageTab *binaryImage, Neurone *workingNeurone)
{
    int absIndex, ordIndex;

    //Score of each pixel of the image
    int pixelSum = 0;

    //Sum of significative weights
    int weightSum = 0;

    //Weighted sum of the image
    int total = 0;

    for(absIndex = 0; absIndex < workingNeurone->weightAbs; absIndex++)
    {
        for(ordIndex = 0; ordIndex < workingNeurone->weightOrd; ordIndex++)
        {
            pixelSum += workingNeurone->weights[absIndex][ordIndex] * binaryImage->donneesTab[absIndex][ordIndex][RED];

            if(workingNeurone->weights[absIndex][ordIndex] > 0)
            {
                weightSum += workingNeurone->weights[absIndex][ordIndex];
            }
        }
    }

    //Check division by zero
    if(weightSum != 0)
    {
        total = pixelSum / weightSum;
    }

    return total;
}



/* @function
 *      Format a binary image for the neural network
 *      White -> -1
 *      Black -> 1
 * 
 * @param
 *      DonneesImageTab *binaryImage    :   image to format
 * 
 * @return  :   \
 */
void formatImage(DonneesImageTab *binaryImage)
{
    int absIndex, ordIndex;

    for(absIndex = 0; absIndex < binaryImage->largeurImage; absIndex++)
    {
        for(ordIndex = 0; ordIndex < binaryImage->hauteurImage; ordIndex++)
        {
            if(binaryImage->donneesTab[absIndex][ordIndex][RED] == BINARY_BLACK
                && binaryImage->donneesTab[absIndex][ordIndex][GREEN] == BINARY_BLACK
                && binaryImage->donneesTab[absIndex][ordIndex][BLUE] == BINARY_BLACK)
            {
                binaryImage->donneesTab[absIndex][ordIndex][RED] = FORMAT_BLACK;
                binaryImage->donneesTab[absIndex][ordIndex][GREEN] = FORMAT_BLACK;
                binaryImage->donneesTab[absIndex][ordIndex][BLUE] = FORMAT_BLACK;
            }
            else
            {
                binaryImage->donneesTab[absIndex][ordIndex][RED] = FORMAT_WHITE;
                binaryImage->donneesTab[absIndex][ordIndex][GREEN] = FORMAT_WHITE;
                binaryImage->donneesTab[absIndex][ordIndex][BLUE] = FORMAT_WHITE;
            }
        }
    }
}



/* @function
 *      Transforms a grey level image into a binary image
 * 
 * @param
 *      DonneesImageTab *greyImage  :   image to converts
 * 
 * @return  :   \
 */
void binariseImage(DonneesImageTab *greyImage)
{
    int absIndex, ordIndex;

    for(absIndex = 0; absIndex < greyImage->largeurImage; absIndex++)
    {
        for(ordIndex = 0; ordIndex < greyImage->hauteurImage; ordIndex++)
        {
            if(greyImage->donneesTab[absIndex][ordIndex][RED] <= 127)
            {
                greyImage->donneesTab[absIndex][ordIndex][RED] = BINARY_BLACK;
                greyImage->donneesTab[absIndex][ordIndex][GREEN] = BINARY_BLACK;
                greyImage->donneesTab[absIndex][ordIndex][BLUE] = BINARY_BLACK;
            }
            else
            {
                greyImage->donneesTab[absIndex][ordIndex][RED] = BINARY_WHITE;
                greyImage->donneesTab[absIndex][ordIndex][GREEN] = BINARY_BLACK;
                greyImage->donneesTab[absIndex][ordIndex][BLUE] = BINARY_BLACK;
            }
        }
    }
}



/* @function
 *      Trains a neurone with a given image
 * 
 * @param
 *      Neurone *neurone  :   neurone to train
 *      DonneesImageTab *formatImage    :   image to use
 * 
 * @return  :   \
 */
void trainNeurone(Neurone *neurone, DonneesImageTab *formatImage)
{
    int absIndex, ordIndex;

    for(absIndex = 0; absIndex < neurone->weightAbs; absIndex++)
    {
        for(ordIndex = 0; ordIndex < neurone->weightOrd; ordIndex++)
        {
            neurone->weights[absIndex][ordIndex] += formatImage->donneesTab[absIndex][ordIndex][RED];
        }
    }
}