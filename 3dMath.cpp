/*****************************************************************************
									
File: Math.cpp				
Author: B. Jordan, D. Elkins, E. Wood		
Date Created: 16/10/06			
Last Modified: 26/10/06			
Description: This file contains all of our math functions.	

*****************************************************************************/

// 18/10/06- Added collision functions

#include "3dMath.h"

/***************************************************************************************
*																					   *
* Function: CVector3 Vector															   *
* Parameters: CVector3 vPoint1, CVector3 vPoint2									   *
* Description: This returns a vector between two points.							   *
*																					   *
***************************************************************************************/
CVector3 Vector(CVector3 vPoint1, CVector3 vPoint2)
{
	
	CVector3 vVector;

	vVector.x = vPoint1.x - vPoint2.x;
	vVector.y = vPoint1.y - vPoint2.y;
	vVector.z = vPoint1.z - vPoint2.z;

	return vVector;
}

/***************************************************************************************
*																					   *
* Function: CVector3 AddVector														   *
* Parameters: CVector3 vPoint1, CVector3 vPoint2									   *
* Description: Adds two vectors together and returns the result.					   *
*																					   *
***************************************************************************************/
CVector3 AddVector(CVector3 vVector1, CVector3 vVector2)
{
	CVector3 vResult;

	vResult.x = vVector2.x + vVector1.x;
	vResult.y = vVector2.y + vVector1.y;
	vResult.z = vVector2.z + vVector1.z;

	return vResult;
}

/***************************************************************************************
*																					   *
* Function: CVector3 DivideVectorByScaler											   *
* Parameters: CVector3 vPoint1, float Scaler										   *
* Description: Returns the division of a vector by a single number.					   *
*																					   *
***************************************************************************************/
CVector3 DivideVectorByScaler(CVector3 vVector1, float Scaler)
{
	CVector3 vResult;
	
	vResult.x = vVector1.x / Scaler;
	vResult.y = vVector1.y / Scaler;
	vResult.z = vVector1.z / Scaler;

	return vResult;
}

/***************************************************************************************
*																					   *
* Function: CVector3 Cross															   *
* Parameters: CVector3 vPoint1, CVector3 vVector									   *
* Description: Returns the cross product (vector product) of two given vectors.		   *
*																					   *
***************************************************************************************/
CVector3 Cross(CVector3 vVector1, CVector3 vVector2)
{
	CVector3 vCross;

	vCross.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
	vCross.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
	vCross.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

	return vCross;
}

/***************************************************************************************
*																					   *
* Function: float Magnitude															   *
* Parameters: CVector3 vNormal														   *
* Description: Returns the magnitude of a vector.									   *
*																					   *
***************************************************************************************/

float Magnitude(CVector3 vNormal)
{
	return (float)sqrt( (vNormal.x * vNormal.x) + (vNormal.y * vNormal.y) + (vNormal.z * vNormal.z) );
}

/***************************************************************************************
*																					   *
* Function: CVector3 Normalize														   *
* Parameters: CVector3 vPoint1, CVector3 vVector									   *
* Description: Returns a normalize vector											   *
*																					   *
***************************************************************************************/
CVector3 Normalize(CVector3 vNormal)
{
	float magnitude = Magnitude(vNormal);

	vNormal.x /= magnitude;
	vNormal.y /= magnitude;
	vNormal.z /= magnitude;

	return vNormal;
}

/***************************************************************************************
*																					   *
* Function: CVector3 MultiplyVector													   *
* Parameters: CVector3 vVector1, CVector3 vVector2									   *
* Description: Multiplies two vectors and returns the result.						   *
*																					   *
***************************************************************************************/
CVector3 MultiplyVector(CVector3 vVector1, CVector3 vVector2)
{
	CVector3 vResult;

	vResult.x = vVector2.x * vVector1.x;
	vResult.y = vVector2.y * vVector1.y;
	vResult.z = vVector2.z * vVector1.z;

	return vResult;
}

/***************************************************************************************
*																					   *
* Function: CVector3 Normal															   *
* Parameters: CVector3 vPolygon[]													   *
* Description: Returns the normal of a polygon.										   *
*																					   *
***************************************************************************************/
CVector3 Normal(CVector3 vPolygon[])					
{
	CVector3 vVector1 = vPolygon[2] - vPolygon[0];
	CVector3 vVector2 = vPolygon[1] - vPolygon[0];

	CVector3 vNormal = Cross(vVector1, vVector2);		// Return a perpendicular vector

	vNormal = Normalize(vNormal);						// Normalize the normal

	return vNormal;	
}

/***************************************************************************************
*																					   *
* Function: float Dot																   *
* Parameters: CVector3 vVector1, CVector3 vVector2									   *
* Description: Calculates and returns a dot product from two vectors.				   *
*																					   *
***************************************************************************************/
float Dot(CVector3 vVector1, CVector3 vVector2) 
{

	return ( (vVector1.x * vVector2.x) + (vVector1.y * vVector2.y) + (vVector1.z * vVector2.z) );
}

/***************************************************************************************
*																					   *
* Function: float Absolute															   *
* Parameters: float num																   *
* Description: Returns the absolute value of a number.								   *
*																					   *
***************************************************************************************/
float Absolute(float num)
{
	if(num < 0)
		return (0 - num);

	return num;
}

/***************************************************************************************
*																					   *
* Function: float Distance															   *
* Parameters: CVector3 vPoint1, CVector3 vPoint2									   *
* Description: Return the distance between two points.								   *
*																					   *
***************************************************************************************/
float Distance(CVector3 vPoint1, CVector3 vPoint2)
{

	double distance = sqrt( (vPoint2.x - vPoint1.x) * (vPoint2.x - vPoint1.x) +
						    (vPoint2.y - vPoint1.y) * (vPoint2.y - vPoint1.y) +
						    (vPoint2.z - vPoint1.z) * (vPoint2.z - vPoint1.z) );

	return (float)distance;
}

/***************************************************************************************
*																					   *
* Function: float Distance															   *
* Parameters: CVector3 Normal, CVector3 Point										   *
* Description: This returns the distance between a plane and an origin.				   *
*																					   *
***************************************************************************************/					
float PlaneDistance(CVector3 Normal, CVector3 Point)
{	
	
	// Where D represents distance:
	//
	// (Ax + By + Cz + D = 0)
	// D = -(Ax + By + Cz)

	float distance = 0;

	distance = - ((Normal.x * Point.x) + (Normal.y * Point.y) + (Normal.z * Point.z));

	return distance;
}

/***************************************************************************************
*																					   *
* Function: double AngleBetweenVectors												   *
* Parameters: CVector3 Vector1, CVector3 Vector2									   *
* Description: This checks to see if a point is inside the ranges of a polygon.		   *
*																					   *
***************************************************************************************/
double AngleBetweenVectors(CVector3 Vector1, CVector3 Vector2)
{							
	
	// Where q represents theta:
	//
	// Cos q = (A � B) / (|A| � |B|)
	// q = arcCos ((A � B) / (|A| � |B|))

	// The two vector's dot product
	float dotProduct = Dot(Vector1, Vector2);		

	// Store the product of both vector's magnitudes
	float vectorsMagnitude = Magnitude(Vector1) * Magnitude(Vector2) ;

	// Store the angle in radians between the 2 vectors - Thank god for math.h :)
	double angle = acos( dotProduct / vectorsMagnitude );

	if(_isnan(angle))
		return 0;			// The angle is indefinite

	return( angle );
	
}

/***************************************************************************************
*																					   *
* Function: bool IntersectedPlane													   *
* Parameters: CVector3 vPoly[], CVector3 vLine[], CVector3 &vNormal,				   *
*			  float &originDistance												 	   *
* Description: Checks to see if a line intersects a plane.							   *
*																					   *
***************************************************************************************/
										
bool IntersectedPlane(CVector3 vPoly[], CVector3 vLine[], CVector3 &vNormal, float &originDistance)
{
	// The distances from the 2 points of the line from the plane
	float distance1=0; 
	float distance2=0;
			
	vNormal = Normal(vPoly);									// Store the normal of the plane

	// Store the distance the plane is from the origin
	originDistance = PlaneDistance(vNormal, vPoly[0]);

	// Store the distance from point1 from the plane
	distance1 = ((vNormal.x * vLine[0].x)  +					// Ax +
		         (vNormal.y * vLine[0].y)  +					// Bx +
				 (vNormal.z * vLine[0].z)) + originDistance;	// Cz + D
	
	// Store the distance from point2 from the plane
	distance2 = ((vNormal.x * vLine[1].x)  +					// Ax +
		         (vNormal.y * vLine[1].y)  +					// Bx +
				 (vNormal.z * vLine[1].z)) + originDistance;	// Cz + D

	// If we multiply both distances and the resultant variable is negative, we have collided
	if(distance1 * distance2 >= 0)
	   return false;
					
	return true;												// Line intersected the plane
}

/***************************************************************************************
*																					   *
* Function: CVector3 ClosestPointOnLine												   *
* Parameters: CVector3 vA, CVector3 vB, CVector3 vPoint							 	   *
* Description: This returns the point on a line that is closest to the vPoint.		   *
*			   vA and vB represent the origin(O) and point(P) of the line.			   *
*																					   *
***************************************************************************************/
CVector3 ClosestPointOnLine(CVector3 vA, CVector3 vB, CVector3 vPoint)
{
	// Create the vector from point vA to vPoint.
	CVector3 vVector1 = vPoint - vA;

	// Create a normalized direction vector from vA to vB
    CVector3 vVector2 = Normalize(vB - vA);

	// Find the distance of the line segment
    float d = Distance(vA, vB);

	// Project the vVector1 onto the vector vVector2.
    float t = Dot(vVector2, vVector1);

	// If the projected distance from vA, "t", is less than or equal to 0 
    if (t <= 0) 
		return vA;		// It must be closest to the end point vA .: Return vA.

	// If our projected distance from vA, "t", is greater than or equal to d
    if (t >= d) 
		return vB;		// It must be closest to the end point vB. .: return vB.
 
	// Create a vector that is of length t and in the direction of vVector2
    CVector3 vVector3 = vVector2 * t;

	// Add vVector3 to the original end point vA to find the closest point on the line.  
    CVector3 vClosestPoint = vA + vVector3;

	return vClosestPoint;
}


/***************************************************************************************
*																					   *
* Function: bool InsidePolygon														   *
* Parameters: CVector3 vIntersection, CVector3 Poly[], long verticeCount		 	   *
* Description: Checks to see if a point is inside the ranges of a polygon.			   *
*																					   *
***************************************************************************************/
bool InsidePolygon(CVector3 vIntersection, CVector3 Poly[], long verticeCount)
{
	const double MATCH_FACTOR = 0.99;			// Floating point fix
	double Angle = 0.0;							// Angle
	CVector3 vA, vB;							// Temp vectors
	
	for (int i = 0; i < verticeCount; i++)		// Go in a circle to each vertex and get the angle between
	{	
		vA = Poly[i] - vIntersection;			// Subtract the intersection point from the current vertex
												
												// Subtract the point from the next vertex
		vB = Poly[(i + 1) % verticeCount] - vIntersection;
												
		Angle += AngleBetweenVectors(vA, vB);	// Find angle between the 2 vectors and add them each loop
	}
											
	if(Angle >= (MATCH_FACTOR * (2.0 * PI)) )	// If the angle is greater than 2 PI,
		return true;							// Point is inside polygon
		
	return false;								// Point not inside polygon
}

/***************************************************************************************
*																					   *
* Function: bool IntersectedPolygon													   *
* Parameters: CVector3 vPoly[], CVector3 vLine[], int verticeCount				 	   *
* Description: Checks if a line is intersecting a polygon.							   *
*																					   *
***************************************************************************************/
bool IntersectedPolygon(CVector3 vPoly[], CVector3 vLine[], int verticeCount)
{
	CVector3 vNormal;
	float originDistance = 0;

	// Make sure the line intersects the plane
	if(!IntersectedPlane(vPoly, vLine,   vNormal,   originDistance))
		return false;

	// Normal and distance passed back from IntersectedPlane()
	// Use it to calculate the intersection point.  
	CVector3 vIntersection = IntersectionPoint(vNormal, vLine, originDistance);

	// Test if intersection point is inside the polygon
	if(InsidePolygon(vIntersection, vPoly, verticeCount))
		return true;							// Collision detected
	return false;								// There was no collision
}

/***************************************************************************************
*																					   *
* Function: CVector3 IntersectionPoint												   *
* Parameters: CVector3 vNormal, CVector3 vLine[], double distance				 	   *
* Description: Returns the intersection point of the line that intersects the plane.   *
*																					   *
***************************************************************************************/									
CVector3 IntersectionPoint(CVector3 vNormal, CVector3 vLine[], double distance)
{
	CVector3 vPoint;							// Point direction
	CVector3 vLineDir;							// Line direction

	double Numerator = 0.0, Denominator = 0.0, dist = 0.0;

	// Get the vector of a line, then normalize it
	vLineDir = vLine[1] - vLine[0];				// Get the Vector of the line
	vLineDir = Normalize(vLineDir);				// Normalize the line's vector


	// Find the distance from one point to the plane.
	Numerator = - (vNormal.x * vLine[0].x +
				   vNormal.y * vLine[0].y +
				   vNormal.z * vLine[0].z + distance);

	// Get the dot product of the line's vector and the normal of the plane
	Denominator = Dot(vNormal, vLineDir);

	if( Denominator == 0.0)						// Check that the divisor does not equal 0
		return vLine[0];						// Return an arbitrary point on the line

	dist = Numerator / Denominator;				// Divide to get the multiplying (percentage) factor
	
	// Multiply the dist by the vector, then add our arbitrary point.
	vPoint.x = (float)(vLine[0].x + (vLineDir.x * dist));
	vPoint.y = (float)(vLine[0].y + (vLineDir.y * dist));
	vPoint.z = (float)(vLine[0].z + (vLineDir.z * dist));

	return vPoint;								// Return the intersection
}


/***************************************************************************************
*																					   *
* Function: int ClassifySphere														   *
* Parameters: CVector3 &vCenter, CVector3 &vNormal, CVector3 &vPoint, float radius,    *
*			  float &distance				 										   *
* Description: This tells if a sphere is BEHIND, in FRONT, or INTERSECTS a plane,      *
*			   and it's distance.													   *
*																					   *
***************************************************************************************/
int ClassifySphere(CVector3 &vCenter, 
				   CVector3 &vNormal, CVector3 &vPoint, float radius, float &distance)
{
	// Find the distance the polygon plane is from the origin.
	float d = (float)PlaneDistance(vNormal, vPoint);

	// Find the distance the center point of the sphere is from the polygon's plane.  
	distance = (vNormal.x * vCenter.x + vNormal.y * vCenter.y + vNormal.z * vCenter.z + d);

	// If the absolute value of the distance is less than the radius, the sphere intersected the plane.
	if(Absolute(distance) < radius)
		return INTERSECTS;
	
	// Else, If the distance is greater than or equal to the radius,
	// the sphere is completely in FRONT of the plane.
	else if(distance >= radius)
		return FRONT;
	
	// If the sphere isn't intersecting or in FRONT of the plane, it must be BEHIND
	return BEHIND;
}


/***************************************************************************************
*																					   *
* Function: bool SpherePolygonCollision												   *
* Parameters: CVector3 vNormal, CVector3 vLine[], double distance				 	   *
* Description: Returns TRUE if our sphere collides with the polygon passed in.		   *
*																					   *
***************************************************************************************/
bool SpherePolygonCollision(CVector3 vPolygon[], 
							CVector3 &vCenter, int vertexCount, float radius)
{
	// Find the normal of the polygon
	CVector3 vNormal = Normal(vPolygon);

	// Store the distance the sphere is from the plane
	float distance = 0.0f;

	// Classify the sphere
	int classification = ClassifySphere(vCenter, vNormal, vPolygon[0], radius, distance);

	// If the sphere intersects the polygon's plane, we need to check further
	if(classification == INTERSECTS) 
	{
		// Project the sphere's center onto the polygon's plane
		CVector3 vOffset = vNormal * distance;

		// Subtract the offset of the plane from the center of the sphere.
		// .: vPosition stores the point that lies on the plane of the polygon.
		CVector3 vPosition = vCenter - vOffset;

		// Pass in the intersection point, the list of vertices and vertex count of the poly.
		if(InsidePolygon(vPosition, vPolygon, 3))
			return true;	// Collision detected
		else
		{
			// Check collision against the edges of the polygon.
			if(EdgeSphereCollision(vCenter, vPolygon, vertexCount, radius))
			{
				return true;	// Collision detected
			}
		}
	}

	return false;
}

/***************************************************************************************
*																					   *
* Function: bool EdgeSphereCollision												   *
* Parameters: CVector3 &vCenter, CVector3 vPolygon[], int vertexCount, float radius    *
* Description: Returns TRUE if the sphere intersects any of the edges of the polygon.  *
*																					   *
***************************************************************************************/
bool EdgeSphereCollision(CVector3 &vCenter, 
						 CVector3 vPolygon[], int vertexCount, float radius)
{
	CVector3 vPoint;

	// Go through all polygon vertices
	for(int i = 0; i < vertexCount; i++)
	{
		// Return the closest point on the current edge to the center of the sphere.
		vPoint = ClosestPointOnLine(vPolygon[i], vPolygon[(i + 1) % vertexCount], vCenter);
		
		// Calculate the distance between the closest point and the center
		float distance = Distance(vPoint, vCenter);

		// If the distance is less than the radius, there must be a collision
		if(distance < radius)
			return true;		// Collision detection.
	}

	return false;				// No collision detection.
}

/***************************************************************************************
*																					   *
* Function: CVector3 GetCollisionOffset												   *
* Parameters: CVector3 &vCenter, CVector3 vPolygon[], int vertexCount, float radius    *
* Description: Returns TRUE if the sphere intersects any of the edges of the polygon.  *
*																					   *
***************************************************************************************/

CVector3 GetCollisionOffset(CVector3 &vNormal, float radius, float distance)
{
	CVector3 vOffset = CVector3(0, 0, 0);

	// If  distance is greater than zero, we're in front of the polygon
	if(distance > 0)
	{
		float distanceOver = radius - distance;	// Sphere overlap distance
		vOffset = vNormal * distanceOver;		// Direction vector to move sphere
	}
	else // Colliding from behind the polygon
	{
		float distanceOver = radius + distance;
		vOffset = vNormal * -distanceOver;
	}

	// Return the offset to move back to
	return vOffset;
}