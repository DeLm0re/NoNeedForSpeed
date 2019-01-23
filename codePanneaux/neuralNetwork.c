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


