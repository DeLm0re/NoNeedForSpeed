#include "trajectory.h"

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
float angleTrajectory(float heightHorizon, float abscisseArrival, float abscisseCar)
{
	//		   . <--- point of arrival in the horizon line
	//         |\
	//opposite>| \
	//		   |  \_   
	//2pi _____|___\_\<-angleTrajectory_____ 0 
	//           ^ ^ 
	//  adjacent_| |
	//             |_point of the car's position
	
	float adjacent, opposite, angleTrajectory ; 
	
	opposite = heightHorizon ; 
	if(abscisseArrival <= abscisseCar)
	{
		adjacent = abscisseCar - abscisseArrival ; 
		angleTrajectory = M_PI - acos( adjacent/opposite ) ; 
	}
	else
	{
		adjacent = abscisseArrival - abscisseCar ; 
		angleTrajectory = acos( adjacent/opposite ) ; 
	}
	return( angleTrajectory ) ; 
}


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
float abscisseTrajectory(float heightHorizon, float angleArrival, float abscisseCar)
{  
	return( abscisseCar + cos(angleArrival) * heightHorizon ) ;
}
