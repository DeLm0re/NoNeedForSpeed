#include "rescale.h"

/* @fonction		:	Take a rectangular image and make it square
 *
 *
 * @param			:
 *      image		: 	The origin image
 *
 * @return		 	:	The origin image with the same size to every borders by adding white pixel
 */
DonneesImageTab* squareImage(DonneesImageTab* image)
{
    // increments of matrix
    int line, column, lineImage, columnImage ;
    // size of original image sqared.
    int sizeSquare ;
    // gap between the width and height of original image
    int gap ;
    // Limits for the three for loops to make the squareImage
    int firstLineLimit, secondLineLimit, thirdLineLimit ;
    int firstLineStart, secondLineStart, thirdLineStart ;
    int firstColumnLimit, secondColumnLimit, thirdColumnLimit ;
    int firstColumnStart, secondColumnStart, thirdColumnStart ;
    // the return squared image
    DonneesImageTab* squareImage ;
    // we take the value of the longest size between width and height
    if ( image->largeurImage < image->hauteurImage )
    {
        sizeSquare = image->hauteurImage ;
        gap = image->hauteurImage - image->largeurImage ;

        firstLineStart = 0 ;
        firstLineLimit = sizeSquare ;

        secondLineStart = 0 ;
        secondLineLimit = sizeSquare ;

        thirdLineStart = 0 ;
        thirdLineLimit = sizeSquare ;

        firstColumnStart = 0 ;
        firstColumnLimit = gap/2 ;

        secondColumnStart = firstColumnLimit ;
        secondColumnLimit = gap/2 + image->largeurImage ;

        thirdColumnStart = secondColumnLimit;
        thirdColumnLimit = image->largeurImage + gap;
    }
    else
    {
        sizeSquare = image->largeurImage ;
        gap = image->largeurImage - image->hauteurImage ;

        firstLineStart = 0 ;
        firstLineLimit = gap/2 ;

        secondLineStart = firstLineLimit ;
        secondLineLimit = gap/2 + image->hauteurImage ;

        thirdLineStart = secondLineLimit ;
        thirdLineLimit = gap + image->hauteurImage ;

        firstColumnStart = 0;
        firstColumnLimit = sizeSquare ;

        secondColumnStart = 0 ;
        secondColumnLimit = sizeSquare ;

        thirdColumnStart = 0 ;
        thirdColumnLimit = sizeSquare ;
    }
    // we set the return squared image
    squareImage = initTab(sizeSquare, sizeSquare) ;

    //printf("first for\n");
    // we fill the gap on the left or the top of the image with white color
    for(line=firstLineStart ; line<firstLineLimit ; line+=1)
    {
        for(column=firstColumnStart ; column<firstColumnLimit ; column+=1)
        {
            squareImage->donneesTab[column][line][BLUE]  = 255 ;
            squareImage->donneesTab[column][line][GREEN] = 255 ;
            squareImage->donneesTab[column][line][RED]   = 255 ;
        }
    }
    // we transfer the origin image on the squared image
    // set of increments of origin image matrix
    //printf("second for\n");
    lineImage = 0 ;
    columnImage = 0 ;
    for(line=secondLineStart ; line<secondLineLimit ; line+=1)
    {
        for(column=secondColumnStart ; column<secondColumnLimit ; column+=1)
        {
            // We reset the increment when we are out of the squared image
            if(lineImage == image->hauteurImage)
            {
                lineImage = 0 ;
            }
            if(columnImage == image->largeurImage)
            {
                    columnImage = 0 ;
                    lineImage+=1 ;
            }
            squareImage->donneesTab[column][line][BLUE]  = image->donneesTab[columnImage][lineImage][BLUE] ;
            squareImage->donneesTab[column][line][GREEN] = image->donneesTab[columnImage][lineImage][GREEN] ;
            squareImage->donneesTab[column][line][RED]   = image->donneesTab[columnImage][lineImage][RED] ;
            columnImage+=1 ;
        }
    }
    //printf("third for\n");
    // we fill the gap on the right or the bottom of the squared image with the white color
    for(line=thirdLineStart ; line<thirdLineLimit ; line+=1)
    {
        for(column=thirdColumnStart; column<thirdColumnLimit ; column+=1)
        {
            squareImage->donneesTab[column][line][BLUE] = 255;
            squareImage->donneesTab[column][line][GREEN] = 255;
            squareImage->donneesTab[column][line][RED] = 255 ;
        }
    }
    return squareImage ;
}

/* @fonction		:	Take an image and resize it to the size put in parameter
 *
 *
 * @param			:
 *      image		: 	The origin image
 *      newSize     :   The size of the returned image
 *
 * @return		 	:	The origin image with the size put in parameter
 */
DonneesImageTab* rescale(DonneesImageTab* image, int newSize)
{
    // The image put in parameter must be square
    int sizeImage = image->hauteurImage ;
    // the returned resized image
    DonneesImageTab* resizedImage ;
    // increments for the matrix image and the matrix resized image
    int lineImage, columnImage, lineResizedImage, columnResizedImage ;
    //  setting of the resized image
    resizedImage = initTabRegion(newSize, newSize) ;

    for(lineImage=0 ; lineImage<sizeImage ; lineImage+=1)
    {
        for(columnImage=0 ; columnImage<sizeImage ; columnImage+=1)
        {
            lineResizedImage   = nmap((float) lineImage, (float) 0, (float) sizeImage, (float) 0, (float) newSize) ;
            columnResizedImage = nmap((float) columnImage, (float) 0, (float) sizeImage, (float) 0, (float) newSize) ;
            //printf("%d\n", lineResizedImage) ;
            //printf("%d\n", columnResizedImage) ;
            resizedImage->donneesTab[columnResizedImage][lineResizedImage][BLUE] = image->donneesTab[columnImage][lineImage][BLUE] ;
            resizedImage->donneesTab[columnResizedImage][lineResizedImage][GREEN] = image->donneesTab[columnImage][lineImage][GREEN] ;
            resizedImage->donneesTab[columnResizedImage][lineResizedImage][RED] = image->donneesTab[columnImage][lineImage][RED] ;
        }
    }
    if(sizeImage<newSize)
    {
        fillGap(resizedImage) ;
    }
    return resizedImage ;
}

/* @fonction		:	Take a rescaled image and fill the hole inside
 *
 *
 * @param			:
 *      image		: 	The origin image
 *
 * @return		 	:	The origin image with hole filled
 */
void fillGap(DonneesImageTab* image)
{
    // increments to browse the image
    int rowImage, columnImage ;
    // size of the image
    int sizeImage = image->largeurImage ;
    int offsetX = 0 ;
    int offsetY = 0 ;

    for(rowImage=0 ; rowImage<sizeImage ; rowImage+=1)
    {
        for(columnImage=0 ; columnImage<sizeImage ; columnImage+=1)
        {
            if(UNCHECKED == image->donneesTab[columnImage][rowImage][BLUE] )
            {
                for(offsetX=-1 ; offsetX<=1 ; offsetX+=1)
                {
                    for(offsetY=-1 ; offsetY<=1 ; offsetY+=1)
                    {
                        if( 0<=rowImage+offsetY && rowImage+offsetY<sizeImage
                        && 0<=columnImage+offsetX && columnImage+offsetX<sizeImage)
                        {
                            if(UNCHECKED != image->donneesTab[columnImage+offsetX][rowImage+offsetY][BLUE])
                            {
                                image->donneesTab[columnImage][rowImage][BLUE] =
                                image->donneesTab[columnImage+offsetX][rowImage+offsetY][BLUE] ;

                                image->donneesTab[columnImage][rowImage][GREEN] =
                                image->donneesTab[columnImage+offsetX][rowImage+offsetY][GREEN] ;

                                image->donneesTab[columnImage][rowImage][RED] =
                                image->donneesTab[columnImage+offsetX][rowImage+offsetY][RED] ;
                            }
                        }
                    }
                }
            }
        }
    }
}
