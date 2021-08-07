#include <stdafx.h>

#include "Texture.h"

//Texture* Texture::s_Instance = NULL;

Texture::Texture() {

}

Texture::~Texture() {
	
}

void Texture::Init(char *filename) {
	int width = 0;
	int height = 0;
	int bpp = 0;

//	printf("%s\n", filename);
	char *imageData = LoadTGA(filename, &width, &height, &bpp);
//	printf("%d %d %d\n", width, height, bpp);

	//load the image data into OpenGL ES texture resource
	if (bpp == 24) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	}
	// free the client memory
	delete[] imageData;

	//set the filters for minification and magnification
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// generate the mipmap chain
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::Init(char *filename, int id) {
	int width = 0;
	int height = 0;
	int bpp = 0;
//	printf("%s\n", filename);

	char *imageData = LoadTGA(filename, &width, &height, &bpp);
//	printf("%d %d %d\n", width, height, bpp);

	if (imageData) {
//		printf("h\n");
	}
	int subWidth = width / 6;
	int subHeight = height / 5;
	int subBpp = bpp / 8;

	if (id >= 24) {
		id -= 24;
	}
	else if (id >= 18) {
		id = id - 12;
	}
	else if (id >= 12) {
		id = id;
	}
	else if (id >= 6){
		id += 12;
	}
	else {
		id += 24;
	}

	char* imageSub = new char[subWidth * subHeight * subBpp];

	for (int i = 0; i < subHeight; i++) {
		memcpy(imageSub + i * subWidth * subBpp, imageData + width * subBpp * i + subWidth * subBpp * (id % 6) + width * subHeight * subBpp * (id / 6), subWidth * subBpp); // top
	}

	//load the image data into OpenGL ES texture resource
	if (bpp == 24) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, subWidth, subHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageSub);
	}
	// free the client memory
	delete[] imageSub;
	delete[] imageData;

	//set the filters for minification and magnification
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// generate the mipmap chain
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::loadCube(char* filename) {
	int width = 0;
	int height = 0;
	int bpp = 0;

	char *imageData = LoadTGA(filename, &width, &height, &bpp);

	int subWidth = width / 4;
	int subHeight = height / 3;
	int subBpp = bpp / 8;

	char **imageSub = new char*[6];
	for (int i = 0; i < 6; i++) {
		imageSub[i] = new char[subWidth * subHeight * subBpp];
	}
	for (int i = 0; i < subHeight; i++) {
		memcpy(imageSub[2] + i * subWidth * subBpp, imageData + subWidth * subBpp + width * subBpp*i, subWidth * subBpp); // top
		memcpy(imageSub[1] + i * subWidth * subBpp, imageData + width * subBpp*i + width * subBpp * subHeight, subWidth * subBpp); // left
		memcpy(imageSub[4] + i * subWidth * subBpp, imageData + subWidth * subBpp + width * subBpp*i + width * subBpp * subHeight, subWidth * subBpp); // front
		memcpy(imageSub[0] + i * subWidth * subBpp, imageData + subWidth * subBpp * 2 + width * subBpp*i + width * subBpp * subHeight, subWidth * subBpp); //right
		memcpy(imageSub[5] + i * subWidth * subBpp, imageData + subWidth * subBpp * 3 + width * subBpp*i + width * subBpp * subHeight, subWidth * subBpp); // back
		memcpy(imageSub[3] + i * subWidth * subBpp, imageData + subWidth * subBpp + width * subBpp*i + width * subBpp * subHeight * 2, subWidth * subBpp); // bot
	}
	for (int i = 0; i < 6; i++) {
		if (bpp == 24) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, subWidth, subHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageSub[i]);
		}
		else {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, subWidth, subHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageSub[i]);
		}
		delete[] imageSub[i];
	}
	// free the client memory
	delete[] imageSub;
	delete[] imageData;
	//set the filters for minification and magnification
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// generate the mipmap chain
}
//Texture* Texture::GetInstance() {
//	if (!s_Instance)
//		s_Instance = new Texture();
//	return s_Instance;
//}