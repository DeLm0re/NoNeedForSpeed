#include "image.h"

// create a 3*3 LBP of the given DonneesImageTab
DonneesImageTab* localBinaryPattern(DonneesImageTab* tabImage);
// comapre two LBP images and return the relative error between tabImage and tabRef
float binaryPatternComp(DonneesImageTab* tabImage, DonneesImageTab* tabRef);
