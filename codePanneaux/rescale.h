#include "region.h"


/* @fonction		:	Take an image and resize it to the size put in parameter
 *
 *
 * @param			:
 *      image		: 	The origin image
 *      newSize     :   The size of the returned image
 *
 * @return		 	:	The origin image with the size put in parameter
 */
DonneesImageTab* rescale(DonneesImageTab* image, int newSize) ;

/* @fonction		:	Take a rectangular image and make it square
 *
 *
 * @param			:
 *      image		: 	The origin image
 *
 * @return		 	:	The origin image with the same size to every borders by adding white pixel
 */
DonneesImageTab* squareImage(DonneesImageTab* image) ;

/* @fonction		:	Take a rescaled image and fill the hole inside
 *
 *
 * @param			:
 *      image		: 	The origin image
 *
 * @return		 	:	The origin image with hole filled
 */
void fillGap(DonneesImageTab* image) ; 
