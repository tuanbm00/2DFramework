#include <stdafx.h>

#include "Model.h"
#include <stdio.h>


Model::Model() {

}

Model::~Model(){
	
}

void Model::Init(char *filename) {
	FILE* file = fopen(filename, "r");
	int numberOfVertices;
	fscanf(file, "NrVertices: %d\n", &numberOfVertices);
	numOfVertices = numberOfVertices;
	vertices = new Vertex[numberOfVertices];
	for (int i = 0; i < numberOfVertices; ++i)
	{
		fscanf(file, "   %*d. pos:[%f, %f, %f]; norm:[%f, %f, %f]; binorm:[%*f, %*f, %*f]; tgt:[%*f, %*f, %*f]; uv:[%f, %f];\n",
			&vertices[i].position.x, &vertices[i].position.y, &vertices[i].position.z,
			&vertices[i].normal.x, &vertices[i].normal.y, &vertices[i].normal.z,
			&vertices[i].uv.x, &vertices[i].uv.y);
	}

	int numberOfIndices;
	fscanf(file, "NrIndices: %d\n", &numberOfIndices);
	indices = new int[numberOfIndices];
	numOfIndices = numberOfIndices;
	for (int i = 0; i < numberOfIndices; i += 3)
	{
		fscanf(file, "   %*d.    %d,    %d,    %d\n", &indices[i], &indices[i + 1], &indices[i + 2]);
	}
	fclose(file);
}

void Model::loadHeight(char* filename) {
		int width = 0;
		int height = 0;
		int bpp = 0;

		char *imageData = LoadTGA(filename, &width, &height, &bpp);

		for (int i = 0; i < numOfVertices; i++) {
			int x = int(vertices[i].uv.x * width);
			int y = int(vertices[i].uv.y * height);
			vertices[i].position.y = imageData[x*width * bpp/8 + y * bpp/8] * GLfloat(0.05);
		}
		float *d = new float[numOfVertices];
		int m = int(sqrt(numOfVertices));
		int n = int(sqrt(numOfVertices));

		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				float c = vertices[i * m + j].position.y;
				int count = 1;
				if (i > 0) {
					c += vertices[(i-1)*m + j].position.y;
					count++;
				}
				if (j > 0) {
					c += vertices[i * m + j-1].position.y;
					count++;
				}
				if (i < m-1) {
					c += vertices[(i + 1) * m + j].position.y;
					count++;
				}
				if (j < n -1) {
					c += vertices[i * m + j+1].position.y;
					count++;
				}
				d[i * m + j] = c / count;
			}
		}
		for (int i = 0; i < numOfVertices; i++) {
			vertices[i].position.y = d[i];
		}
		
		delete[] d;
		delete[] imageData;
}
