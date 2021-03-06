
//General header files
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//More specific headers files
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

//Header file
#include "neuralNetwork.h"

int main(int argc, char** argv)
{
    DonneesImageRGB* image = NULL;
    // If their is an argument
    if (argc == 2)
	{
	    // We read the image from the first argument and we store it in a DonneesImageRGB.
	    image = lisBMPRGB(argv[1]);
	}
	// Otherwise
	else
	{
	    // We read a default image
	    image = lisBMPRGB("imagesRoutes/panneau2.bmp");
	}
	
	// If we have correctly read the image we can begin
	if (image != NULL)
	{
	    ////////////////////////
	    //--- Inialisation ---//
	    ////////////////////////
	    
	    // Initializing the random with a seed
	    srand(10495854);
	    
	    // We erase all previously detected signs
	    int errorSystem = system("rm -f panneaux/*");
	    if (errorSystem)
	    {
	        printf("Error : couldn't remove files in the following folder : panneaux\n");
	    }
	    
	    //--- Initialization of the neural network wich will detect the different characters
	    printf("Creating the neural network that will detect the characters on the sign\n");
	    AlphabetNeuralNetwork* neuralNetwork = createAlphabetNeuralNetwork();
	    
	    ///////////////////////////
	    //--- Basic Variables ---//
	    ///////////////////////////
	    
	    char pathStepImage [50]; // use to store the path of the image for the detected signs

        //////////////////////////
	    //--- Sign detection ---//
	    //////////////////////////
        
        printf("Setting up the image\n");
        // We convert the image we will analyse to a DonneesImageTab
        DonneesImageTab* imageTab = RGBToTab(image);
        
        // We apply a median filter to it, so we lower the noize
        DonneesImageTab* imageTabMedian = applyMedianFilterOnTab(imageTab, 5, 5);
	
	    // We create a new DonneesImageTab that will contain the regions on the image
	    DonneesImageTab* tabRegion = initTabRegion(image->largeurImage, image->hauteurImage);
	    // We find all regions that correspond to the sign color we want
	    // color exemples : green(0, 140, 120) white(110, 110, 110) orange(140, 90, 10)
	    printf("Finding regions...\n");
	    IdRegions* idRegions = findAllColorRegionBottomUp(imageTabMedian, tabRegion, 0, 140, 120, 50);
	    // We print the number of regions we found
	    printf("Number of regions found : %d\n", idRegions->size);
	
	    // We read the reference image that we will use to compare linear binary patern
	    DonneesImageRGB* imageRef = lisBMPRGB("imagesRoutes/LBPReference/referenceLBP1.bmp");
	    // We convert it to a DonneesImageTab
        DonneesImageTab* imageRefTab = RGBToTab(imageRef);
        
        // We set up some basic variable for the next step (finding the sign)
	    DonneesImageTab* currentShape = NULL; // Will be use to store a potential sign
	    DonneesImageTab* LBPShape = NULL; // Will be use to store his LBP version
	    DonneesImageRGB* imageRegionShape = NULL; // Will be use to convert it to a BMP image
	    float error = 0; // Will be use to store the error between the LBP reference and the LBP
	    // And we set up some basic variable for the character detections
	    DonneesImageTab* imageRegionSign = NULL; // Will be use to store all the regions we detect in a sign
	    IdRegions* idRegionsSign = NULL; // Will be use to store the ID of each of the region found on the sign
	    int indexChar = 0; // Will be use to check every character we found on the sign
	    DonneesImageTab* currentCharacter = NULL; // Will be use to store the potential character, so we can test it
	    DonneesImageTab* squaredCharacter = NULL; // Will be use to transform the character to a square image
	    DonneesImageTab* rescaledCharacter = NULL; // Will be use to store the rescale character
	    int idCharacter = -1; // Will be use to store the ID of the found character
	    // For each detected regions
	    int i;
	    for(i = 0; i < idRegions->size; i++)
	    {
	        // We extract the shape of it from the original image
	        currentShape = getOriginalShape(imageTab, tabRegion,  idRegions->regions[i]);
	        // We change it to a grey level image
	        makeGreyLevel(currentShape);
	        // We convert it to an LBP image
	        LBPShape = applyLocalBinaryPattern(currentShape);
	        // We calculate the error between this LBP image and the reference one
	        error = binaryPatternComp(LBPShape, imageRefTab);
	        // If the error is positive (no problem) and if it is bellow 0.00008 (seems to be a good value)
	        if (0 < error && error <= 0.00008)
	        {
	            printf(" The region %d seems to be a sign\n", i);
	            // Then we concider that this region is a sign, so :
	            // We reset the path of the image
	            strcpy(pathStepImage, "");
	            // We create the new one
	            sprintf(pathStepImage, "panneaux/%d - sign.bmp", i);
	            // We convert the current region to a DonneesImageRGB
	            imageRegionShape = tabToRGB(currentShape);
	            // And then we write the BMP image
	            ecrisBMPRGB_Dans(imageRegionShape, pathStepImage);
	            
	            printf("  Finding characters\n");
	            //--- Then we try to find all the character in the supposed sign
	            // First, we apply some filters on the image we found so it is easier to use
	            cutBetweenLevel(currentShape, 100, 255);
	            cutBetweenLevel(currentShape, 0, 100);
                // We set the DonneesImageTab that will contain the regions of the current signS
	            imageRegionSign = initTabRegion(currentShape->largeurImage, currentShape->hauteurImage);
                // We find all the regions
	            idRegionsSign = findAllRegionBottomUp(currentShape, imageRegionSign, 200);
	            printf("  %d potential characters found\n", idRegionsSign->size);
	            
	            //--- Letter detection
	            printf("  Detecting wich character are real one and putting the word back together\n");
	            printf("    The word written on the sign is : ");
	            // For each potential characters found on the sign
	            for(indexChar = 0; indexChar < idRegionsSign->size; indexChar++)
	            {
	                // We extract it to a temporary variables
	                currentCharacter = getShape(imageRegionSign, idRegionsSign->regions[indexChar]);
	                // If it is a valid shape
	                if (currentCharacter != NULL)
	                {
	                    // We binarise it
	                    regionToBinary(currentCharacter);
	                    // We square it
	                    squaredCharacter = squareImage(currentCharacter);
	                    // And rescale it so it can be used by the neural network
	                    rescaledCharacter = rescale(squaredCharacter, NB_INPUTS_NEURONE);
	                    // We find which letter it is
	                    idCharacter = detectLetterOnImage(neuralNetwork, rescaledCharacter);
	                    // If we found the letter
	                    if (idCharacter != -1)
	                    {
	                        // We write it down as a char
	                        printf("%c", idCharacter + 65);
	                    }
	                    // We free the modified shape
	                    libereDonneesTab(&squaredCharacter);
	                    libereDonneesTab(&rescaledCharacter);
	                }
	                // We free the current character shape
	                libereDonneesTab(&currentCharacter);
	            }
	            // Once we finished writing the word marked on the sign, we do a cariage return 
	            printf("\n");
                // Then, we free everything
	            libereDonneesTab(&imageRegionSign);
	            destructIdRegions(&idRegionsSign);
	        }
	        // Then we free the memory before the next loop
	        libereDonneesTab(&currentShape);
	        libereDonneesTab(&LBPShape);
	    }
	    
        // Finaly we free everything
        libereDonneesImageRGB(&image);
        libereDonneesTab(&imageTab);
        
        libereDonneesTab(&imageTabMedian);
        
        libereDonneesTab(&tabRegion);
        destructIdRegions(&idRegions);
        
        libereDonneesImageRGB(&imageRef);
        libereDonneesTab(&imageRefTab);
    }
    
	return 0;
}
