#include "main.h"
#include "loadModel.h"
#include "3dMath.h"

int gBuffer[50000] = {0};

CLoadModel::CLoadModel()
{
	m_FilePointer = NULL;
}


bool CLoadModel::Import3DS(t3DModel *pModel, char *strFileName)
{
	char strMessage[255] = {0};
	tChunk currentChunk = {0};

	m_FilePointer = fopen(strFileName, "rb");

	if(!m_FilePointer) 
	{
		MessageBox(NULL, strMessage, "Error loading stage model.", MB_OK);
		return false;
	}

	ReadChunk(&currentChunk);

	if (currentChunk.ID != PRIMARY)
	{
		MessageBox(NULL, strMessage, "Invalid stage model.", MB_OK);
		return false;
	}
	ProcessNextChunk(pModel, &currentChunk);

	ComputeNormals(pModel);

	CleanUp();

	return true;
}

void CLoadModel::CleanUp()
{
	if (m_FilePointer) {
		fclose(m_FilePointer);
		m_FilePointer = NULL;
	}
}

void CLoadModel::ProcessNextChunk(t3DModel *pModel, tChunk *pPreviousChunk)
{
	t3DObject newObject = {0};
	tMaterialInfo newTexture = {0};

	tChunk currentChunk = {0};
	tChunk tempChunk = {0};		

	while (pPreviousChunk->bytesRead < pPreviousChunk->length)
	{

		ReadChunk(&currentChunk);

		switch (currentChunk.ID)
		{
		case VERSION:

			currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);

			break;

		case OBJECTINFO:
			{	

			ReadChunk(&tempChunk);

			tempChunk.bytesRead += fread(gBuffer, 1, tempChunk.length - tempChunk.bytesRead, m_FilePointer);

			currentChunk.bytesRead += tempChunk.bytesRead;

			ProcessNextChunk(pModel, &currentChunk);
			break;
		}
		case MATERIAL:

			pModel->numOfMaterials++;
 
			pModel->pMaterials.push_back(newTexture);

			ProcessNextMaterialChunk(pModel, &currentChunk);
			break;

		case OBJECT:

			pModel->numOfObjects++;
		
			pModel->pObject.push_back(newObject);
			
			memset(&(pModel->pObject[pModel->numOfObjects - 1]), 0, sizeof(t3DObject));

			currentChunk.bytesRead += GetString(pModel->pObject[pModel->numOfObjects - 1].strName);
			
			ProcessNextObjectChunk(pModel, &(pModel->pObject[pModel->numOfObjects - 1]), &currentChunk);
			break;

		case EDITKEYFRAME:

			currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
			break;

		default: 

			currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
			break;
		}

		pPreviousChunk->bytesRead += currentChunk.bytesRead;
	}
}

void CLoadModel::ProcessNextObjectChunk(t3DModel *pModel, t3DObject *pObject, tChunk *pPreviousChunk)
{
	tChunk currentChunk = {0};

	while (pPreviousChunk->bytesRead < pPreviousChunk->length)
	{
		ReadChunk(&currentChunk);

		switch (currentChunk.ID)
		{
		case OBJECT_MESH:
		
			ProcessNextObjectChunk(pModel, pObject, &currentChunk);
			break;

		case OBJECT_VERTICES:
			ReadVertices(pObject, &currentChunk);
			break;

		case OBJECT_FACES:
			ReadVertexIndices(pObject, &currentChunk);
			break;

		case OBJECT_MATERIAL:
			
			ReadObjectMaterial(pModel, pObject, &currentChunk);			
			break;

		case OBJECT_UV:	

			ReadUVCoordinates(pObject, &currentChunk);
			break;

		default:  

			currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
			break;
		}

		pPreviousChunk->bytesRead += currentChunk.bytesRead;
	}
}

void CLoadModel::ProcessNextMaterialChunk(t3DModel *pModel, tChunk *pPreviousChunk)
{
	tChunk currentChunk = {0};

	while (pPreviousChunk->bytesRead < pPreviousChunk->length)
	{
		ReadChunk(&currentChunk);

		switch (currentChunk.ID)
		{
		case MATNAME:							

			currentChunk.bytesRead += fread(pModel->pMaterials[pModel->numOfMaterials - 1].strName, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
			break;

		case MATDIFFUSE:

			ReadColorChunk(&(pModel->pMaterials[pModel->numOfMaterials - 1]), &currentChunk);
			break;
		
		case MATMAP:
			
			ProcessNextMaterialChunk(pModel, &currentChunk);
			break;

		case MATMAPFILE:

			currentChunk.bytesRead += fread(pModel->pMaterials[pModel->numOfMaterials - 1].strFile, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
			break;
		
		default:  

			currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
			break;
		}

		pPreviousChunk->bytesRead += currentChunk.bytesRead;
	}
}

void CLoadModel::ReadChunk(tChunk *pChunk)
{
	pChunk->bytesRead = fread(&pChunk->ID, 1, 2, m_FilePointer);
	pChunk->bytesRead += fread(&pChunk->length, 1, 4, m_FilePointer);
}

int CLoadModel::GetString(char *pBuffer)
{
	int index = 0;

	fread(pBuffer, 1, 1, m_FilePointer);

	while (*(pBuffer + index++) != 0) {

		fread(pBuffer + index, 1, 1, m_FilePointer);
	}

	return strlen(pBuffer) + 1;
}

void CLoadModel::ReadColorChunk(tMaterialInfo *pMaterial, tChunk *pChunk)
{
	tChunk tempChunk = {0};

	ReadChunk(&tempChunk);

	tempChunk.bytesRead += fread(pMaterial->color, 1, tempChunk.length - tempChunk.bytesRead, m_FilePointer);

	pChunk->bytesRead += tempChunk.bytesRead;
}

void CLoadModel::ReadVertexIndices(t3DObject *pObject, tChunk *pPreviousChunk)
{
	unsigned short index = 0;

	pPreviousChunk->bytesRead += fread(&pObject->numOfFaces, 1, 2, m_FilePointer);

	pObject->pFaces = new tFace [pObject->numOfFaces];
	memset(pObject->pFaces, 0, sizeof(tFace) * pObject->numOfFaces);

	for(int i = 0; i < pObject->numOfFaces; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			pPreviousChunk->bytesRead += fread(&index, 1, sizeof(index), m_FilePointer);

			if(j < 3)
			{
				pObject->pFaces[i].vertIndex[j] = index;
			}
		}
	}
}

void CLoadModel::ReadUVCoordinates(t3DObject *pObject, tChunk *pPreviousChunk)
{
	pPreviousChunk->bytesRead += fread(&pObject->numTexVertex, 1, 2, m_FilePointer);

	pObject->pTexVerts = new CVector2 [pObject->numTexVertex];

	pPreviousChunk->bytesRead += fread(pObject->pTexVerts, 1, pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);
}

void CLoadModel::ReadVertices(t3DObject *pObject, tChunk *pPreviousChunk)
{
	pPreviousChunk->bytesRead += fread(&(pObject->numOfVerts), 1, 2, m_FilePointer);

	pObject->pVerts = new CVector3 [pObject->numOfVerts];
	memset(pObject->pVerts, 0, sizeof(CVector3) * pObject->numOfVerts);

	pPreviousChunk->bytesRead += fread(pObject->pVerts, 1, pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);

	for(int i = 0; i < pObject->numOfVerts; i++)
	{
		float fTempY = pObject->pVerts[i].y;

		pObject->pVerts[i].y = pObject->pVerts[i].z;

		pObject->pVerts[i].z = -fTempY;
	}

	
	/*for(int i = 0; i < pObject->numOfVerts; i++)
	{
		float fTempX = pObject->pVerts[i].x;

		pObject->pVerts[i].x = -fTempX;
	}/*

	
	for(int i = 0; i < pObject->numOfVerts; i++)
	{
		float fTempZ = pObject->pVerts[i].z;

		pObject->pVerts[i].z = -fTempZ;
	}*/
}

void CLoadModel::ReadObjectMaterial(t3DModel *pModel, t3DObject *pObject, tChunk *pPreviousChunk)
{
	char strMaterial[255] = {0};
	char opac[255] = {"TRANS.BMP"};

	pPreviousChunk->bytesRead += GetString(strMaterial);

	for(int i = 0; i < pModel->numOfMaterials; i++)
	{
		if(strcmp(strMaterial, pModel->pMaterials[i].strName) == 0)
		{
			if(strlen(pModel->pMaterials[i].strFile) > 0) 
			{				
				pObject->materialID = i;

					if(strcmp(opac, pModel->pMaterials[i].strFile) == 0)
					{
						pObject->bIsTrans = true;
						pObject->bHasTexture = false;
					}
					else
					{

						pObject->bIsTrans = false;
						pObject->bHasTexture = true;
					}

			}
			break;
		}

		else
		{
			if(pObject->bHasTexture != true)
			{
				pObject->materialID = -1;
			}
		}
	}

	pPreviousChunk->bytesRead += fread(gBuffer, 1, pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);
}			







// Mathmatical functions
#define Mag(Normal) (sqrt(Normal.x*Normal.x + Normal.y*Normal.y + Normal.z*Normal.z))

void CLoadModel::ComputeNormals(t3DModel *pModel)
{
	// 27/10/06 - Needs looking at. Make more use of new maths functions.

	CVector3 vVector1, vVector2, vNormal, vPoly[3];

	if(pModel->numOfObjects <= 0)
		return;

	for(int index = 0; index < pModel->numOfObjects; index++)
	{
		t3DObject *pObject = &(pModel->pObject[index]);

		CVector3 *pNormals		= new CVector3 [pObject->numOfFaces];
		CVector3 *pTempNormals	= new CVector3 [pObject->numOfFaces];
		pObject->pNormals		= new CVector3 [pObject->numOfVerts];

		for(int i=0; i < pObject->numOfFaces; i++)
		{												
			vPoly[0] = pObject->pVerts[pObject->pFaces[i].vertIndex[0]];
			vPoly[1] = pObject->pVerts[pObject->pFaces[i].vertIndex[1]];
			vPoly[2] = pObject->pVerts[pObject->pFaces[i].vertIndex[2]];

			vVector1 = Vector(vPoly[0], vPoly[2]);
			vVector2 = Vector(vPoly[2], vPoly[1]);

			vNormal  = Cross(vVector1, vVector2);
			pTempNormals[i] = vNormal;
			vNormal  = Normalize(vNormal);

			pNormals[i] = vNormal;
		}


		CVector3 vSum(0.0, 0.0, 0.0);
		CVector3 vZero = vSum;
		int shared=0;

		int i = 0;

		for (i = 0; i < pObject->numOfVerts; i++)
		{
			for (int j = 0; j < pObject->numOfFaces; j++)
			{
				if (pObject->pFaces[j].vertIndex[0] == i || 
					pObject->pFaces[j].vertIndex[1] == i || 
					pObject->pFaces[j].vertIndex[2] == i)
				{
					vSum = AddVector(vSum, pTempNormals[j]);
					shared++;
				}
			}      
			
			pObject->pNormals[i] = DivideVectorByScaler(vSum, float(-shared));

			pObject->pNormals[i] = Normalize(pObject->pNormals[i]);	

			vSum = vZero;
			shared = 0;
		}

		delete [] pTempNormals;
		delete [] pNormals;
	}
}