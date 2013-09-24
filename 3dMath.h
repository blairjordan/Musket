/*****************************************************************************
									
File: Math.h
Author: B Jordan							
Description: This file contains all of our maths functions and point classes.													 
*****************************************************************************/

#ifndef _3DMATH_H
#define _3DMATH_H

#include <math.h>
#include <float.h>

#define PI 3.1415926535897932

// Sphere classifications
#define BEHIND		0
#define INTERSECTS	1
#define FRONT		2


// 3D point class
class CVector3 
{
public:

	inline CVector3() {}

	// Allows initialization of data upon creating an instance
	inline CVector3(float X, float Y, float Z) 
	{ 
		x = X; y = Y; z = Z;
	}

	//////////////////////////////////////////////////////
	//				Overloaded Operaters				//
	//////////////////////////////////////////////////////

	// + operator overload -Allows vector addition
	inline CVector3 operator+(CVector3 vVector)
	{
		return CVector3(vVector.x + x, vVector.y + y, vVector.z + z);
	}

	// - operator overload -Allows vector subtraction
	inline CVector3 operator-(CVector3 vVector)
	{
		return CVector3(x - vVector.x, y - vVector.y, z - vVector.z);
	}
	
	// * operator overload -Allows multiplication by a scalar
	inline CVector3 operator*(float num)
	{
		return CVector3(x * num, y * num, z * num);
	}

	// / operator overload -Allows division by a scalar
	inline CVector3 operator/(float num)
	{
		return CVector3(x / num, y / num, z / num);
	}
	
	// XYZ coordinates
	float x, y, z;

};

// 2D point class
class CVector2 
{
public:
	float x, y;
};

////////////////////////////////////////////////
//				Math Functions				  //
////////////////////////////////////////////////

CVector3 Normal(CVector3 vPolygon[]);
CVector3 Vector(CVector3 vPoint1, CVector3 vPoint2);
CVector3 AddVector(CVector3 vVector1, CVector3 vVector2);
CVector3 DivideVectorByScaler(CVector3 vVector1, float Scaler);
CVector3 Cross(CVector3 vVector1, CVector3 vVector2);
CVector3 Normalize(CVector3 vNormal);
CVector3 MultiplyVector(CVector3 vVector1, CVector3 vVector2);
CVector3 ClosestPointOnLine(CVector3 vA, CVector3 vB, CVector3 vPoint);
CVector3 IntersectionPoint(CVector3 vNormal, CVector3 vLine[], double distance);
CVector3 GetCollisionOffset(CVector3 &vNormal, float radius, float distance);

int ClassifySphere(CVector3 &vCenter, CVector3 &vNormal, CVector3 &vPoint, float radius, float &distance);

float Dot(CVector3 vVector1, CVector3 vVector2);
float Magnitude(CVector3 vNormal);
float Distance(CVector3 vPoint1, CVector3 vPoint2);
float PlaneDistance(CVector3 Normal, CVector3 Point);
float Absolute(float num);

double AngleBetweenVectors(CVector3 Vector1, CVector3 Vector2);

bool IntersectedPlane(CVector3 vPoly[], CVector3 vLine[], CVector3 &vNormal, float &originDistance);
bool InsidePolygon(CVector3 vIntersection, CVector3 Poly[], long verticeCount);
bool IntersectedPolygon(CVector3 vPoly[], CVector3 vLine[], int verticeCount);
bool SpherePolygonCollision(CVector3 vPolygon[], CVector3 &vCenter, int vertexCount, float radius);
bool EdgeSphereCollision(CVector3 &vCenter, CVector3 vPolygon[], int vertexCount, float radius);

#endif