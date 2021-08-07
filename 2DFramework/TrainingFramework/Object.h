#pragma once
#include <math.h>
#include "Shaders.h"
#include "Texture.h"
#include "Model.h"
#include "Camera.h"
#include "../Utilities/utilities.h"

#define PI 3.1459f


class Object {
public:
	Texture *m_texture;
	Model m_model;
	Shaders m_shaders;
	Matrix m_worldMatrix;
	Matrix m_WVP;

	float m_deltatime;
	int m_iObjectID;
	int m_numOfTexture;
	int m_numOfCube;
	int m_shaderID;
	float xlong;
	GLuint vboId;
	GLuint iboId;
	GLuint *textureID;
	Vector3 m_position;
	Vector3 m_scale;
	Vector3 m_rotation;

	Object();
	~Object();
	void SetID(int id);
	void SetnumOfCube(int numOfCube);
	void SetnumOfTexture(int numOfTexture);
	void SetnumOfShader(int shaderID);
	void SetPosition(float x, float y, float z);
	void SetScale(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void Init(char** fileTexture, char* fileModel, char* fileVS, char* fileFS);
	void loadCube(char* fileModel, char* filename);
	void Draw();
	void Init2D(char* filename, float divW, float divH);
	void Draw2D();
	void InitBullet();
	void DrawBullet();
	void IntMVP();
	void CleanUp();
	void Update(float deltaTime);
};