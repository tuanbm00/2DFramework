#pragma once
#include <math.h>
#include "../Utilities/utilities.h"

#define MOVE_FRONT 1
#define MOVE_BACK 1 << 1
#define MOVE_LEFT 1 << 2
#define MOVE_RIGHT 1 << 3

#define ROTATE_LEFT 1 << 4
#define ROTATE_RIGHT 1 << 5
#define ROTATE_UP 1 << 6
#define ROTATE_DOWN 1 << 7

class Camera {
private:
	static Camera* s_instance;
	float m_FOV;
	float m_Near;
	float m_Far;
	float m_fog_start;
	float m_fog_length;
	float m_move_speed;
	float m_rot_speed;
	float m_rotVertical;
	float m_rotHorizontal;
	float m_moveVertical;
	float m_moveHorizontal;
	float m_limitRotVertical;
	int keyPressed;
	Vector3 m_position;
	Vector3 m_target;
	Vector3 m_up;
	Vector3 m_fog_color;
	Matrix m_viewMatrix;
	Matrix m_worldMatrix;
public:
	Camera();
	~Camera();
	void Init(float m_fov, float m_near, float m_far);
	void SetPosition(float x, float y, float z);
	void SetTarget(float x, float y, float z);
	void SetUp(float x, float y, float z);
	void SetFogStart(float x);
	void SetFogLength(float x);
	void SetFogColor(float x, float y, float z);
	float GetFogStart();
	float GetFogLength();
	Vector3 GetFogColor();
	void SetSpeed(float m_move_speed, float m_rot_speed);
	void Update(float deltatime);
	void Move(char key, bool bIsPressed);
	void CheckMove();
	void CleanUp();
	static Camera* GetInstance();
	Matrix GetProjection();
	Matrix GetPerspective();
	Matrix GetViewMatrix();
	Matrix GetWorldMatrix();
	Matrix RotationMatrixAroundX(float angle);
	Matrix RotationMatrixAroundY(float angle);
};
