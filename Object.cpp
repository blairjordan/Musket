/*****************************************************************************
*																			 *
* File: Object.cpp															 *
* Author: B. Jordan														 	 *
*																			 *
*****************************************************************************/

#include "object.h"
#include "main.h"

// ******************* Setters
void CObject::NumOfVertices(int verts){
	CObject::numOfVerts = verts;
}

void CObject::NumOfFaces(int faces){
	CObject::numOfFaces = faces;
}
void CObject::NumOfTexVertex(int texverts){
	CObject::numTexVertex = texverts;
}

void CObject::HasTexture(bool texture){
	CObject::texture = texture;
}

void CObject::SetVertices(CVector3 *pVerts, float x, float y, float z){
	
	CObject::pVerts->x = x;
	CObject::pVerts->y = y;
	CObject::pVerts->z = z;
}

void CObject::SetVertX(CVector3 *pVerts, float x){

	CObject::pVerts->x = x;

}

void CObject::SetVertY(CVector3 *pVerts, float y){

	CObject::pVerts->y = y;
}

void CObject::SetVertZ(CVector3 *pVerts, float z){

	CObject::pVerts->z = z;
	
}

void CObject::SetTexVertX(CVector2 *pTexVerts, float x){

	CObject::pTexVerts->x = x;
}

void CObject::SetTexVertY(CVector2 *pTexVerts, float y){
	
	CObject::pTexVerts->y = y;
}

// Setters *******************


// ******************* Getters
CVector3 CObject::GetVertices(){ 
	
	CVector3 vertices;

	vertices.x = CObject::pVerts->x;
	vertices.y = CObject::pVerts->y;
	vertices.z = CObject::pVerts->z;

	return vertices;		
}

float CObject::GetTexVertX(){

	float xTexVert = CObject::pTexVerts->x;

	return xTexVert;
}

float CObject::GetTexVertY(){

	float yTexVert = CObject::pTexVerts->y;

	return yTexVert;
}


float CObject::GetVertX(){

	float xVert = CObject::pVerts->x;

	return xVert;
}


float CObject::GetVertY(){

	float yVert = CObject::pVerts->y;

	return yVert;
}

float CObject::GetVertZ(){

	float zVert = CObject::pVerts->z;

	return zVert;
}


// Getters ******************