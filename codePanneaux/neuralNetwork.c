#include "neuralNetwork.h"


/* @function
 *      Creates a neurone with default values
 *      and weights initialized
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
 *      Creates the alphabet neural network of 26 neurones
 *      initialized and trained
 * 
 * @return  :   AlphabetNeuralNetwork created
 * */
AlphabetNeuralNetwork *createAlphabetNeuralNetwork(void)
{
    // Names of letter.bmp files
    char alphabet[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

    //New neural network
    AlphabetNeuralNetwork *ANN = (AlphabetNeuralNetwork*)malloc(sizeof(AlphabetNeuralNetwork));
    ANN->nbNeurone = 26;
    ANN->neurones = (Neurone*)malloc(ANN->nbNeurone * sizeof(Neurone));

    int alphabetIndex;
    for(alphabetIndex = 0; alphabetIndex < ANN->nbNeurone; alphabetIndex++)
    {
        //Creates new neurone
        ANN->neurones[alphabetIndex] = *createNeurone(NB_INPUTS_NEURONE, NB_INPUTS_NEURONE, alphabetIndex + 1);

        //Get the corresponding image to use for training
        char *imagePath = (char*)malloc(30*sizeof(char));
        char *imageName = (char*)malloc(6*sizeof(char));
        imageName[0] = alphabet[alphabetIndex];
        strcat(imageName, ".bmp");
        strcat(imagePath, "alphabet/");
        strcat(imagePath, imageName);

        //Print in the console
        printf("Training neural network on %s\n", imagePath);

        DonneesImageRGB *imageRGB = lisBMPRGB(imagePath);
	    DonneesImageTab *image = RGBToTab(imageRGB);
	    makeGreyLevel(image);
	    binariseImage(image);
	    formatImage(image);

        //Train the neuron
        trainNeurone(&(ANN->neurones[alphabetIndex]), image);
    }

    return ANN;
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
float calculateWeightedSum(DonneesImageTab *binaryImage, Neurone *workingNeurone)
{
    int absIndex, ordIndex;

    //Score of each pixel of the image
    int pixelSum = 0;

    //Sum of significative weights
    int weightSum = 0;

    //Weighted sum of the image
    float total = 0;

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
 * char *imagePath = (char*)malloc(30*sizeof(char));
        char *imageName = (char*)malloc(6*sizeof(char));
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



/* @function
 *      Analyze a binary image of size 60x60
 *      and return the most probable letter detected
 *      
 * @param
 *      AlphabetNeuralNetwork *ANN    :   neural network trained
 *      DonneesImageTab *binaryImage  :   image to analyze
 * 
 * @return  :   most probable letter detected, or -1 if nothing
 */
Letter detectLetterOnImage(AlphabetNeuralNetwork *ANN, DonneesImageTab *binaryImage)
{
    Letter mostProbableLetter = -1;

    int neuroneIndex;
    float bestSum = 0;

    for(neuroneIndex = 0; neuroneIndex < ANN->nbNeurone; neuroneIndex++)
    {
        //Compare the image to the weights of the neuron
        float currentSum = calculateWeightedSum(binaryImage, &(ANN->neurones[neuroneIndex]));
        
        //Get the letter that matches best
        if(currentSum > bestSum)
        {
            bestSum = currentSum;
            mostProbableLetter = ANN->neurones[neuroneIndex].targetLetter;
        }
    }

    //Check if match is enough
    if(bestSum < 0.5)
    {
        mostProbableLetter = -1;
    }
    else
    {
        //printf("Found letter %d with %d percent match\n", mostProbableLetter, (int)(bestSum * 100));
    }


    return mostProbableLetter;
}
