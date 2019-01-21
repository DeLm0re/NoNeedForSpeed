#include "region.h"

// Use to get the crossing point of the two side of a road in tabImage
Point* getRoadPoint(DonneesImageTab* tabImageGradiant);

// returns the crossing point of two lines
Point* getCrossingPoint(Line* line1, Line* line2);

// returns true if the nexAbscissa is on the road, false instead
bool isOnTheRoad(int nextAbscissa, int roadWidth, int horizonPointAbscissa);
