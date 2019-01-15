#include "roadDetection.h"

Point* getRoadPoint(DonneesImageTab* tabImage)
{
    // We create the hough transform of the image
	DonneesImageTab* tabHough = createHough(tabImage, 200, 720);
	printf(" Edit it so it is easier to use\n");
    // Making every point black or white
	cutBetweenLevel(tabHough, 80, 255);
	cutBetweenLevel(tabHough, 0, 80);
	// Making sure to have a blob around each point to find the regions easily with a dillatation filter
	applyDillatationFilter(tabHough, 200);
	
	// We find each point on the hough transform by finding regions using the bottom up method
	DonneesImageTab* tabRegionHough = initTabRegion(tabHough->largeurImage, tabHough->hauteurImage);
	IdRegions* idRegionsHough = findAllRegionBottomUp(tabHough, tabRegionHough, 200);
	
	// We find the two border lines of the road
	Line* line1 = getCenterLineFromRegion(tabHough, tabRegionHough, idRegionsHough->regions[1], 0);
	Line* line2 = getCenterLineFromRegion(tabHough, tabRegionHough, idRegionsHough->regions[2], 0);
	
	// We find the point where the line cross
	Point* point = getCrossingPoint(line1, line2);
	
	libereDonneesTab(&tabHough);
	libereDonneesTab(&tabRegionHough);
	destructIdRegions(&idRegionsHough);
	free(line1);
	free(line2);
    return point;
}

Point* getCrossingPoint(Line* line1, Line* line2)
{
    float maxR1, maxR2;
	float angle1, angle2;
	float radius1, radius2;
    float m1, m2, n1, n2;

    maxR1 = line1->maxAngularIndex/2;
	angle1 = nmap(line1->angularIndex, 0, line1->maxAngularIndex-1, 0, M_PI);
	radius1 = nmap(line1->rIndex, 0, line1->maxRIndex-1, -maxR1, maxR1);
	if (angle1 != 0)
	{
	    m1 = tan(angle1 - M_PI/2);
	    n1 = radius1 / sin(angle1);
	}
	else
	{
	    m1 = NAN;
	    n1 = NAN;
	}
	
	maxR2 = line2->maxAngularIndex/2;
	angle2 = nmap(line2->angularIndex, 0, line2->maxAngularIndex-1, 0, M_PI);
	radius2 = nmap(line2->rIndex, 0, line2->maxRIndex-1, -maxR2, maxR2);
	if (angle2 != 0)
	{
	    m2 = tan(angle2 - M_PI/2);
	    n2 = radius2 / sin(angle2);
	}
	else
	{
	    m2 = NAN;
	    n2 = NAN;
	}
	
	
	Point* point = NULL;
	if((angle1 != 0 || angle2 != 0) && (m1 != m2))
	{
	    point = (Point*) malloc(sizeof(Point));
	    point->x = -1;
	    point->y = -1;
	    point->coef = -1;
	    point->label =-1;
	    if (angle1 == 0)
	    {
	        point->x = radius1;
	        point->y = m2*point->x + n2;
	    }
	    else if (angle2 == 0)
	    {
	        point->x = radius2;
	        point->y = m1*point->x + n1;
	    }
	    else
	    {
	        point->x = (n2 - n1)/(m1 - m2);
	        point->y = m1*point->x + n1;
	    }
	}
	return point;
}
