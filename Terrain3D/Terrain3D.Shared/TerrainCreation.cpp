#include "pch.h"
#include "Game.h"
#include <fstream>
#include <math.h>

void CGame::CreateTerrain(int m)
{
	double temp;
	int size = m*m;
	VERTEX *GridVertices = new VERTEX[size];
	for (int i = 0; i < m; i++)
		for (int j = 0; j < m; j++)
		{
			GridVertices[m * i + j].X = (float)i;
			if (i > 1 && j > 1 && i<511 && j < 511)
				GridVertices[m * i + j].Y = (tempHM[m*i + j] + tempHM[m*(i + 1) + j] + tempHM[m*(i - 1) + j] + tempHM[m*i + j + 1] + tempHM[m*i + j - 1] +
				tempHM[m*(i + 1) + j + 1] + tempHM[m*(i + 1) + j - 1] + tempHM[m*(i - 1) + j + 1] + tempHM[m*(i - 1) + j - 1]) / 9;
			else
				GridVertices[m * i + j].Y = tempHM[m*i + j];
			GridVertices[m * i + j].Z = (float)j;
			GridVertices[m * i + j].NX = 0.0f;
			GridVertices[m * i + j].NY = 1.0f;
			GridVertices[m * i + j].NZ = 0.0f;
			GridVertices[m * i + j].V = GridVertices[m * i + j].X / 511;
			GridVertices[m * i + j].U = GridVertices[m * i + j].Z / 511;
			GridVertices[m * i + j].tV = tileFactor * GridVertices[m * i + j].X / 511 ;
			GridVertices[m * i + j].tU = tileFactor * GridVertices[m * i + j].Z / 511;
		}

	dbgInt += GridVertices[512].U + " " + GridVertices[512].V + GridVertices[512].X + " " + GridVertices[512].Z;
	XMFLOAT3 temp1, temp2, temp3, temp4;
	XMVECTOR result, result2, result3;
	for (int i = m+1; i < m*m-m-1; i++)
	{
		if (i%m != 0 && i%m != (m - 1))
		{
			temp1.x = GridVertices[i].X - GridVertices[i - 1].X;
			temp1.y = GridVertices[i].Y - GridVertices[i - 1].Y;
			temp1.z = GridVertices[i].Z - GridVertices[i - 1].Z;

			temp2.x = GridVertices[i + 1].X - GridVertices[i].X;
			temp2.y = GridVertices[i + 1].Y - GridVertices[i].Y;
			temp2.z = GridVertices[i + 1].Z - GridVertices[i].Z;

			temp3.x = GridVertices[i - m].X - GridVertices[i].X;
			temp3.y = GridVertices[i - m].Y - GridVertices[i].Y;
			temp3.z = GridVertices[i - m].Z - GridVertices[i].Z;

			temp4.x = GridVertices[i].X - GridVertices[i + m].X;
			temp4.y = GridVertices[i].Y - GridVertices[i + m].Y;
			temp4.z = GridVertices[i].Z - GridVertices[i + m].Z;

			result = XMVector3Cross(XMLoadFloat3(&temp3), XMLoadFloat3(&temp1));
			result2 = XMVector3Cross(XMLoadFloat3(&temp4), XMLoadFloat3(&temp2));
			result3 = result + result2;
			result3 = XMVector3Normalize(result3);
			GridVertices[i].NX = XMVectorGetX(result3);
			GridVertices[i].NY = XMVectorGetY(result3);
			GridVertices[i].NZ = XMVectorGetZ(result3);
		}
			
	}


	
	

	D3D11_BUFFER_DESC bd = { 0 };
	bd.ByteWidth = sizeof(VERTEX) * size;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA srd = { GridVertices, 0, 0 };

	dev->CreateBuffer(&bd, &srd, &vertexbuffer);

	int k = 0;
	indicesSize = (m - 1)*(m - 1) * 6;
	int *GridIndices = new int[indicesSize];
	int t = 0;
	while (t < indicesSize)
	{
		GridIndices[t] = k;
		GridIndices[t + 1] = k + 1;
		GridIndices[t + 2] = k + m;
		GridIndices[t + 3] = k + m;
		GridIndices[t + 4] = k + 1;
		GridIndices[t + 5] = k + m + 1;
		k++;
		if (k%m == (m - 1)) k++;
		t += 6;
	}
	D3D11_BUFFER_DESC ibd = { 0 };
	ibd.ByteWidth = sizeof(int) * indicesSize;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA isrd = { GridIndices, 0, 0 };
	dev->CreateBuffer(&ibd, &isrd, &indexbuffer);


	delete GridVertices;
	delete GridIndices;
}