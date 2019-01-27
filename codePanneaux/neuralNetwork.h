#include "rescale.h"
#include <string.h>

/* @definition
 *      Dimentions of the matrix of inputs of a neuron for one letter
 */
#define NB_INPUTS_NEURONE 60

/* @type
 *      Letters of the alphabet and associated values
 */
typedef enum{A = 1, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z = 26}Letter;

/* @definition
 *      Values of a binary image
 */
#define BINARY_BLACK 1
#define BINARY_WHITE 0
#define FORMAT_BLACK 1
#define FORMAT_WHITE -1


/* @type
 *      Structure of a neurone with
 *      undefined number of input
 *      
 * @param
 * 
 *      int *weights  :   table of the weights associated with the inputs
 *      int weightAbs   :   weight dimention in abscissa
 *      int weightOrd   :   weight dimention in ordinate
 *      Letter targetLetter :   letter that this neurone can recognize
 */
typedef struct Neurone 
{
    int **weights;
    int weightAbs;
    int weightOrd;
    Letter targetLetter;
}Neurone;


/* @type
 *      Structure of a neural network for letter detection
 *      Composed of undefined number of Neurone
 * 
 * @param
 *      Neurone *neurones   :  table of Neuron for each letter to  detect
 *      int nbNeurone   :   number of Neuron
 */
typedef struct AlphabetNeuralNetwork
{
    Neurone *neurones;
    int nbNeurone;
}AlphabetNeuralNetwork;


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
Neurone *createNeurone(int weightAbsDimention, int weigthOrdDimention, Letter targetLetter);


/* @function
 *      Creates the alphabet neural network of 26 neurones
 *      initialized and trained
 * 
 * @return  :   AlphabetNeuralNetwork created
 * */
AlphabetNeuralNetwork *createAlphabetNeuralNetwork(void);



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
float calculateWeightedSum(DonneesImageTab *binaryImage, Neurone *workingNeurone);



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
void formatImage(DonneesImageTab *binaryImage);



/* @function
 *      Transforms a grey level image into a binary image
 * 
 * @param
 *      DonneesImageTab *greyImage  :   image to converts
 * 
 * @return  :   \
 */
void binariseImage(DonneesImageTab *greyImage);



/* @function
 *      Trains a neurone with a given image
 * 
 * @param
 *      Neurone *neurone  :   neurone to train
 *      DonneesImageTab *formatImage    :   image to use
 * 
 * @return  :   \
 */
void trainNeurone(Neurone *neurone, DonneesImageTab *formatImage);



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
Letter detectLetterOnImage(AlphabetNeuralNetwork *ANN, DonneesImageTab *binaryImage);



