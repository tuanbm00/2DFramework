#include <stdafx.h>
#include "SceneManager.h"
#include <stdio.h>


SceneManager* SceneManager::s_instance = NULL;

SceneManager::SceneManager() {

}

SceneManager::~SceneManager() {

}

void SceneManager::Init() {
	FILE* file = fopen("..\\Resources\\SceneManager.txt", "r");
	int numberOfCamera;
	char *buff = new char[256];
	float x, y, z;

	fscanf(file, "#Cameras: %d\n", &numberOfCamera);
	for (int i = 0; i < numberOfCamera; i++) {
		int idOfCamera;
		fscanf(file, "ID %d\n", &idOfCamera);
		fscanf(file, "POSITION %f %f %f\n", &x, &y, &z);
		Camera::GetInstance()->SetPosition(x, y, z);

		fscanf(file, "TARGET %f %f %f\n", &x, &y, &z);
		Camera::GetInstance()->SetTarget(x, y, z);

		fscanf(file, "UP %f %f %f\n", &x, &y, &z);
		Camera::GetInstance()->SetUp(x, y, z);

		fscanf(file, "FOVY %f\n", &x);
		fscanf(file, "NEAR %f\n", &y);
		fscanf(file, "FAR %f\n", &z);
		Camera::GetInstance()->Init(x, y, z);

		fscanf(file, "MOVE_SPEED %f\n", &x);
		fscanf(file, "ROTATE_SPEED %f\n", &y);
		Camera::GetInstance()->SetSpeed(x, y);

		fscanf(file, "FOG_START %f\n", &x);
		Camera::GetInstance()->SetFogStart(x);

		fscanf(file, "FOG_LENGTH %f\n", &x);
		Camera::GetInstance()->SetFogLength(x);
		
		fscanf(file, "FOG_COLOR %f %f %f\n", &x, &y, &z);
		Camera::GetInstance()->SetFogColor(x, y, z);
	}

	fscanf(file, "\n", buff);
	fscanf(file, "#Objects: %d\n", &m_numOfObject);
	m_listObject = new Object[m_numOfObject];
	for (int i = 0; i < m_numOfObject; i++) {
		int idObject, numOfCube, numOfTextures, shaderID;
		fscanf(file, "ID %d\n", &idObject);
		m_listObject[i].SetID(idObject);
		fscanf(file, "MODEL %d\n", &idObject);
		fscanf(file, "TEXTURES %d\n", &numOfTextures);
		m_listObject[i].SetnumOfTexture(numOfTextures);
		for (int j = 0; j < numOfTextures; j++) {
			fscanf(file, "TEXTURE %d\n", &idObject);
		}

		fscanf(file, "CUBETEXTURES %d\n", &numOfCube);
		m_listObject[i].SetnumOfCube(numOfCube);

		fscanf(file, "SHADER %d\n", &shaderID);
		m_listObject[i].SetnumOfShader(shaderID);

		fscanf(file, "POSITION %f %f %f\n", &x, &y, &z);
		m_listObject[i].SetPosition(x, y, z);

		fscanf(file, "ROTATION %f %f %f\n", &x, &y, &z);
		m_listObject[i].SetRotation(x, y, z);

		fscanf(file, "SCALE %f %f %f\n", &x, &y, &z);
		m_listObject[i].SetScale(x, y, z);
		fscanf(file, "\n", buff);
	}
	delete[] buff;
	fclose(file);
}

SceneManager* SceneManager::GetInstance() {
	if (!s_instance)
		s_instance = new SceneManager();
	return s_instance;
}

Object* SceneManager::GetListObject() {
	return m_listObject;
}

int SceneManager::GetNumOfObject() {
	return m_numOfObject;
}

void SceneManager::Draw() {
	for (int i = 0; i < m_numOfObject; i++) {
		m_listObject[i].Draw();
	}
}

void SceneManager::CleanUP() {
	for (int i = 0; i < m_numOfObject; i++) {
		m_listObject[i].CleanUp();
	}
	if (s_instance) {
		delete s_instance;
		s_instance = NULL;
	}
	Camera::GetInstance()->CleanUp();
}

void SceneManager::MemoryClear() {
	for (int i = 0; i < m_numOfObject; i++) {
		delete[] m_listObject[i].m_texture;
		delete[] m_listObject[i].textureID;
	}
	delete[] m_listObject;

	if (s_instance) {
		delete s_instance;
		s_instance = NULL;
	}
}

void SceneManager::Update(float deltatime) {
	for (int i = 0; i < m_numOfObject; i++) {
		m_listObject[i].Update(deltatime);
	}
	Camera::GetInstance()->Update(deltatime);
}

void SceneManager::MoveCamera(char key, bool bIsPressed) {
	Camera::GetInstance()->Move(key, bIsPressed);
}
