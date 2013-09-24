/*****************************************************************************
*																			 *
* File: Object.h															 *
* Author: B. Jordan														 	 *
*																			 *
* Description: TODO															 *
*																			 *
*****************************************************************************/

#ifndef _OBJECT_H
#define _OBJECT_H

#include "main.h"
#include "loadModel.h"

class CObject{
	
public:

	// ******************* Setters

	void NumOfVertices(int numOfVerts);
	void NumOfFaces(int numOfVerts);
	void NumOfTexVertex(int numTexVertex);
	void HasTexture(bool texture);

	// 3d vertex point setters
	void SetVertices(CVector3 *pVerts, float x, float y, float z);
	void SetVertX(CVector3 *pVerts, float x);
	void SetVertY(CVector3 *pVerts, float y);
	void SetVertZ(CVector3 *pVerts, float z);

	// 2d vertex point setters -- used for our textures
	void SetTexVertices(CVector2 *pTexVerts, float x, float y);
	void SetTexVertX(CVector2 *pTexVerts, float x);
	void SetTexVertY(CVector2 *pTexVerts, float y);
	
	// Setters *******************


	// ******************* Getters

	inline int NumOfVertices(){		return CObject::numOfVerts;    }
	inline int NumOfFaces(){  		return CObject::numOfFaces;    }
	inline int NumOfTexVertex(){	return CObject::numTexVertex;  }
	inline bool HasTexture(){  		return CObject::texture; 	   }

	// 3d vertex point getters
	CVector3 GetVertices();
	float GetVertX();
	float GetVertY();
	float GetVertZ();
	
	// 2d vertex point getters
	float GetTexVertX();
	float GetTexVertY();

	// Getters ******************

private:

	int numOfVerts;
	int numOfFaces;
	int  numTexVertex;
	int  materialID;
	bool texture;
	char strName[255];
	CVector3  *pVerts;
	CVector3  *pNormals;
	CVector2  *pTexVerts;
	tFace	  *pFaces;

	CLoadModel model;
};

#endif