// Faire des putain de liste chainé pour les regions

#include "image.h"
#include <string.h>

#ifndef BORDER
	#define BORDER -1
#endif
#ifndef UNCHECKED
	#define UNCHECKED -2
#endif

//please, a chained list
typedef struct {
	// Color of the region
	int blue;
	int green;
	int red;
	// position of the corresponding seed
	int x;
	int y;
	// id of the region
	int label;
} IdRegion;

typedef struct {
	IdRegion** regions;
	int size;
} IdRegions;

// Returns a DonneesImageTab with all his value set to UNCHECKED
DonneesImageTab* initTabRegion(int width, int height);

/* PAS APPEL*/
// Initialisation function
IdRegion* initIdRegion(int blue, int green, int red, int x, int y, int label);
IdRegions* initIdRegions(int size);
// Destruction function
void destructIdRegions(IdRegions** idRegions);

// Put the region which the pixel (x, y) is from into tabRegion. The region is in the color of the given pixel.
//returns the color of the said region
IdRegion* findRegionBottomUp(DonneesImageTab* tabImage, DonneesImageTab* tabRegion, int x, int y, int label, int sensibility);
// Find all the regions in the image using the BottomUp algorithm and put them in tabRegion
//return all the idRegion of the different region
IdRegions* findAllRegionBottomUp(DonneesImageTab* tabImage, DonneesImageTab* tabRegion, int sensibility);
// Find all the regions of a given color in the image using the BottomUp algorithm and put them in tabRegion
//return all the idRegion of the different region
IdRegions* findAllColorRegionBottomUp(DonneesImageTab* tabImage, DonneesImageTab* tabRegion, int red, int green, int blue, int sensibility);

/* PAS APPEL*/
// Give the color of a neighboor
IdRegion* whatIsNeighboorsColor(DonneesImageTab* tabRegion, int x, int y);

DonneesImageTab* getShape(DonneesImageTab* tabRegion,  IdRegion* idRegion);

DonneesImageTab* getOriginalShape(DonneesImageTab* tabImage, DonneesImageTab* tabRegion,  IdRegion* idRegion);

void regionToBinary(DonneesImageTab* tabImage);

void writeAllRegion(DonneesImageTab *imageRegion, IdRegions *allRegions);
