#include <stdafx.h>
#include "ResourceManager.h"
#include "SceneManager.h"
#include <stdio.h>

ResourceManager* ResourceManager::s_instance = NULL;

ResourceManager::ResourceManager() {

}

ResourceManager::~ResourceManager() {

}

ResourceManager* ResourceManager::GetInstance() {
	if (!s_instance) 
		s_instance = new ResourceManager();
	return s_instance;
}

void ResourceManager::CleanUp() {
	if (s_instance) {
		delete s_instance;
		s_instance = NULL;
	}
}

void ResourceManager::Init() {
	int numOfObject = SceneManager::GetInstance()->GetNumOfObject();
	Object *listObject = SceneManager::GetInstance()->GetListObject();
	FILE* file = fopen("..\\Resources\\ResourceManager.txt", "r");
	char *buff = new char[256];
	int numOfModels, numOfTextures, numOfCube, numOfShader;
	fscanf(file, "#Models: %d\n", &numOfModels);
	m_listModelFile = new char*[numOfModels];
	for (int i = 0; i < numOfModels; i++) {
		m_listModelFile[i] = new char[256];
		int id;
		fscanf(file, "ID %d\n", &id);
		fscanf(file, "FILE %s\n", m_listModelFile[i]);
	}
	fscanf(file, "\n", buff);

	fscanf(file, "#2D Textures: %d\n", &numOfTextures);
	m_listTextureFile = new char*[numOfTextures];
	for (int i = 0; i < numOfTextures; i++) {
		m_listTextureFile[i] = new char[256];
		int id;
		fscanf(file, "ID %d\n", &id);
		fscanf(file, "FILE %s\n", m_listTextureFile[i]);
		fgets(buff, 255, file);
		fgets(buff, 255, file);
	}
	fscanf(file, "\n", buff);

	fscanf(file, "#Cube Textures: %d\n", &numOfCube);
	m_listCubeFile = new char*[numOfCube];
	for (int i = 0; i < numOfCube; i++) {
		m_listCubeFile[i] = new char[256];
		int id;
		fscanf(file, "ID %d\n", &id);
		fscanf(file, "FILE %s\n", m_listCubeFile[i]);
		fgets(buff, 255, file);
		fgets(buff, 255, file);
	}

	fscanf(file, "\n", buff);
	fscanf(file, "#Shaders: %d\n", &numOfShader);
	m_listShader = new char*[numOfShader*2];
	for (int i = 0; i <numOfShader; i++) {
		m_listShader[2*i] = new char[256];
		m_listShader[2*i+1] = new char[256];
		int id;
		fscanf(file, "ID %d\n", &id);
		fscanf(file, "%s\n", m_listShader[2*i]);
		fscanf(file, "%s\n", m_listShader[2*i+1]);
		fgets(buff, 255, file);
		fgets(buff, 255, file);
	}
	fclose(file);

	int count = 0;
	for (int i = 0; i < numOfObject; i++) {
		int id = listObject[i].m_iObjectID;
		if (listObject[i].m_numOfCube == 0) {
			char** texture = new char*[listObject[i].m_numOfTexture];
			
			for (int j = 0; j < listObject[i].m_numOfTexture; j++) {
				texture[j] = m_listTextureFile[count];
				count++;
			}
			listObject[i].Init(texture, m_listModelFile[id], m_listShader[2*listObject[i].m_shaderID], m_listShader[2*listObject[i].m_shaderID +1]);
			delete[] texture;
		}
		else {
			listObject[i].loadCube(m_listModelFile[id], m_listCubeFile[0]);
		}
	}
	for (int i = 0; i < numOfModels; i++) {
		delete[] m_listModelFile[i];
	}
	for (int i = 0; i < numOfTextures; i++) {
		delete[] m_listTextureFile[i];
	}
	for (int i = 0; i < numOfCube; i++) {
		delete[] m_listCubeFile[i];
	}
	for (int i = 0; i < numOfShader*2; i++) {
		delete[] m_listShader[i];
	}
	delete[] m_listShader;
	delete[] m_listCubeFile;
	delete[] m_listModelFile;
	delete[] m_listTextureFile;
	delete[] buff;
}
