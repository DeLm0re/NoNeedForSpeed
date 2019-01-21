#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>


/* @fonction		:	Return the angle in radian of the car's trajectory from the horizon point and the positin of the car
 *						
 * 
 * @param			:
 *    heightHorizon		: 	Height of the horizon's line
 *    abscisseArrival	: 	Abscisse of the arrival point where the car must go
 *    abscisseCar		:	Abscisse of the position of the car 
 * 
 * @retour		 	:	Angle in radian of the trajectory line of the car will go
 */
float angleTrajectory(float heightHorizon, float abscisseArrival, float abscisseCar); 


/* @fonction		:	Return the abscisse of the horizon point
 *						
 * 
 * @param		:
 *    heightHorizon	: 	Height of the horizon's line
 *    angleArrival	: 	Angle of the trajectory where the car will go
 *    abscisseCar	:	Abscisse of the position of the car 
 * 
 * @retour		 	:	Abscisse of the horizon's point where the car must go
 */
float abscisseTrajectory(float heightHorizon, float angleArrival, float abscisseCar) ; 
