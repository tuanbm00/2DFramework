#include "stdafx.h"
#include "Camera.h"
#include "Globals.h"
#include<iostream>

Camera* Camera::s_instance = NULL;

Camera::Camera() {
	keyPressed = 0;	
	m_moveVertical = 0;
	m_moveHorizontal = 0;
	m_rotVertical = 0;
	m_rotHorizontal = 0;
	m_limitRotVertical = 0;
}

Camera::~Camera() {

}

void Camera::Init(float m_fov, float m_near, float m_far) {
	m_FOV = m_fov;
	m_Near = m_near;
	m_Far = m_far;
}

void Camera::SetPosition(float x, float y, float z) {
	m_position = Vector3(x, y, z);
}

void Camera::SetTarget(float x, float y, float z) {
	m_target = Vector3(x, y, z);
}

void Camera::SetUp(float x, float y, float z) {
	m_up = Vector3(x, y, z);
}

void Camera::SetFogStart(float x) {
	m_fog_start = x;
}

void Camera::SetFogLength(float x) {
	m_fog_length = x;
}

void Camera::SetFogColor(float x, float y, float z) {
	m_fog_color = Vector3(x, y, z);
}

float Camera::GetFogStart() {
	return m_fog_start;
}

float Camera::GetFogLength() {
	return m_fog_length;
}

Vector3 Camera::GetFogColor() {
	return m_fog_color;
}

void Camera::SetSpeed(float move_speed, float rot_speed) {
	m_move_speed = move_speed;
	m_rot_speed = rot_speed;
}

void Camera::Update(float deltatime) {
	CheckMove();
	Vector3 m_move = Vector3(0, 0, 0);
	if (m_moveVertical == 1) {
		m_move += (m_position - m_target).Normalize();
	} 
	if (m_moveVertical == -1) {
		m_move += - (m_position - m_target).Normalize();
	}
	if (m_moveHorizontal == 1) {
		m_move += (m_up.Cross((m_position - m_target).Normalize())).Normalize();
	}
	if (m_moveHorizontal == -1) {
		m_move += -(m_up.Cross((m_position - m_target).Normalize())).Normalize();
	}
	
	m_position += m_move * deltatime*m_move_speed;
	m_target += m_move * deltatime*m_move_speed;
	
	Vector4 localTarget = Vector4(0, 0, -(m_position - m_target).Length(), 1);
	localTarget = localTarget * RotationMatrixAroundY(m_rotHorizontal*deltatime*m_rot_speed);
	localTarget = localTarget * GetWorldMatrix();
	m_target.x = localTarget.x;
	m_target.y = localTarget.y;
	m_target.z = localTarget.z;

	if (m_limitRotVertical + m_rotVertical * deltatime*m_rot_speed < 1.57f && m_limitRotVertical + m_rotVertical * deltatime*m_rot_speed > -1.57f) {
		localTarget = Vector4(0, 0, -(m_position - m_target).Length(), 1);
		localTarget = localTarget * RotationMatrixAroundX(m_rotVertical*deltatime*m_rot_speed);
		localTarget = localTarget * GetWorldMatrix();
		m_limitRotVertical += m_rotVertical * deltatime*m_rot_speed;

		m_target.x = localTarget.x;
		m_target.y = localTarget.y;
		m_target.z = localTarget.z;
	}
}

Camera* Camera::GetInstance() {
	if (!s_instance)
		s_instance = new Camera();
	return s_instance;
}

Matrix Camera::GetPerspective() {
	float aspect = (float)Globals::screenWidth / Globals::screenHeight;
	Matrix Perspective;
	Perspective.SetPerspective(m_FOV, aspect, m_Near, m_Far);
	return Perspective;
}

Matrix Camera::GetProjection() {
	Matrix S, T, P;
	float left = -Globals::screenWidth, right = Globals::screenWidth, top = -Globals::screenHeight, bottom = Globals::screenHeight, nearc = m_Near, farc = m_Far;
	S.m[0][0] = float (2) / (right - left);
	S.m[0][1] = 0;
	S.m[0][2] = 0;
	S.m[0][3] = 0;

	S.m[1][0] = 0;
	S.m[1][1] = float (2) / (top - bottom);
	S.m[1][2] = 0;
	S.m[1][3] = 0;

	S.m[2][0] = 0;
	S.m[2][1] = 0;
	S.m[2][2] = float (-2) / (farc - nearc);
	S.m[2][3] = 0;

	S.m[3][0] = 0;
	S.m[3][1] = 0;
	S.m[3][2] = 0;
	S.m[3][3] = 1;

	T.m[0][0] = 1;
	T.m[0][1] = 0;
	T.m[0][2] = 0;
	T.m[0][3] = -(left + right) / 2;

	T.m[1][0] = 0;
	T.m[1][1] = 1;
	T.m[1][2] = 0;
	T.m[1][3] = -(top + bottom) / 2;

	T.m[2][0] = 0;
	T.m[2][1] = 0;
	T.m[2][2] = -1;
	T.m[2][3] = -(farc + nearc) / 2;

	T.m[3][0] = 0;
	T.m[3][1] = 0;
	T.m[3][2] = 0;
	T.m[3][3] = 1;

	P = S * T;

	return P;
}

Matrix Camera::GetWorldMatrix() {
	Vector3 xaxis, yaxis, zaxis;
	zaxis = (m_position - m_target).Normalize();
	xaxis = (m_up.Cross(zaxis)).Normalize();
	yaxis = (zaxis.Cross(xaxis)).Normalize();
	Matrix R, T;
	R.m[0][0] = xaxis.x;
	R.m[0][1] = xaxis.y;
	R.m[0][2] = xaxis.z;
	R.m[0][3] = 0;

	R.m[1][0] = yaxis.x;
	R.m[1][1] = yaxis.y;
	R.m[1][2] = yaxis.z;
	R.m[1][3] = 0;

	R.m[2][0] = zaxis.x;
	R.m[2][1] = zaxis.y;
	R.m[2][2] = zaxis.z;
	R.m[2][3] = 0;

	R.m[3][0] = 0;
	R.m[3][1] = 0;
	R.m[3][2] = 0;
	R.m[3][3] = 1;

	T.m[0][0] = 1;
	T.m[0][1] = 0;
	T.m[0][2] = 0;
	T.m[0][3] = 0;

	T.m[1][0] = 0;
	T.m[1][1] = 1;
	T.m[1][2] = 0;
	T.m[1][3] = 0;

	T.m[2][0] = 0;
	T.m[2][1] = 0;
	T.m[2][2] = 1;
	T.m[2][3] = 0;

	T.m[3][0] = m_position.x;
	T.m[3][1] = m_position.y;
	T.m[3][2] = m_position.z;
	T.m[3][3] = 1;
	
	m_worldMatrix = R * T;
	return m_worldMatrix;
}

Matrix Camera::GetViewMatrix() {
	Vector3 xaxis, yaxis, zaxis;
	zaxis = (m_position - m_target).Normalize();
	xaxis = (m_up.Cross(zaxis)).Normalize();
	yaxis = (zaxis.Cross(xaxis)).Normalize();
	m_viewMatrix.m[0][0] = xaxis.x;
	m_viewMatrix.m[0][1] = yaxis.x;
	m_viewMatrix.m[0][2] = zaxis.x;
	m_viewMatrix.m[0][3] = 0;

	m_viewMatrix.m[1][0] = xaxis.y;
	m_viewMatrix.m[1][1] = yaxis.y;
	m_viewMatrix.m[1][2] = zaxis.y;
	m_viewMatrix.m[1][3] = 0;

	m_viewMatrix.m[2][0] = xaxis.z;
	m_viewMatrix.m[2][1] = yaxis.z;
	m_viewMatrix.m[2][2] = zaxis.z;
	m_viewMatrix.m[2][3] = 0;

	m_viewMatrix.m[3][0] = -m_position.Dot(xaxis);
	m_viewMatrix.m[3][1] = -m_position.Dot(yaxis);
	m_viewMatrix.m[3][2] = -m_position.Dot(zaxis);
	m_viewMatrix.m[3][3] = 1;

	return m_viewMatrix;
}

Matrix Camera::RotationMatrixAroundX(float angle) {
	Vector3 xaxis, yaxis, zaxis;
	zaxis = (m_position - m_target).Normalize();
	xaxis = (m_up.Cross(zaxis)).Normalize();
	Vector4 localX = Vector4(xaxis, 0);
	localX = localX * GetViewMatrix();
	Matrix rotationAxis;
	return rotationAxis.SetRotationAngleAxis(angle, localX.x, localX.y, localX.z);
}

Matrix Camera::RotationMatrixAroundY(float angle) {
	Vector3 xaxis, yaxis, zaxis;
	zaxis = (m_position - m_target).Normalize();
	xaxis = (m_up.Cross(zaxis)).Normalize();
	yaxis = (zaxis.Cross(xaxis)).Normalize();
	Vector4 localY = Vector4(0, 1, 0, 0);
	localY = localY * GetViewMatrix();
	Matrix rotationAxis;
	return rotationAxis.SetRotationAngleAxis(angle, localY.x, localY.y, localY.z);
}

void Camera::Move(char key, bool bIsPress) {
	if (bIsPress) {
		switch (key) {
			case 'W':
			case 'w':
				keyPressed = keyPressed | MOVE_FRONT;
				break;
			case 'S':
			case 's':
				keyPressed = keyPressed | MOVE_BACK;
				break;
			case 'A':
			case 'a':
				keyPressed = keyPressed | MOVE_LEFT;
				break;
			case 'D':
			case 'd':
				keyPressed = keyPressed | MOVE_RIGHT;
				break;
			case '\&':
				keyPressed = keyPressed | ROTATE_UP;
				break;
			case '\(':
				keyPressed = keyPressed | ROTATE_DOWN;
				break;
			case '\%':
				keyPressed = keyPressed | ROTATE_LEFT;
				break;
			case '\'':
				keyPressed = keyPressed | ROTATE_RIGHT;
				break;
		}
	}
	else {
		switch (key) {
		case 'W':
		case 'w':
			keyPressed = keyPressed ^ MOVE_FRONT;
			m_moveVertical = 0;
			break;
		case 'S':
		case 's':
			keyPressed = keyPressed ^ MOVE_BACK;
			m_moveVertical = 0;
			break;
		case 'A':
		case 'a':
			keyPressed = keyPressed ^ MOVE_LEFT;
			m_moveHorizontal = 0;
			break;
		case 'D':
		case 'd':
			keyPressed = keyPressed ^ MOVE_RIGHT;
			m_moveHorizontal = 0;
			break;
		case '\&':
			keyPressed = keyPressed ^ ROTATE_UP;
			m_rotVertical = 0;
			break;
		case '\(':
			keyPressed = keyPressed ^ ROTATE_DOWN;
			m_rotVertical = 0;
			break;
		case '\%':
			keyPressed = keyPressed ^ ROTATE_LEFT;
			m_rotHorizontal = 0;
			break;
		case '\'':
			keyPressed = keyPressed ^ ROTATE_RIGHT;
			m_rotHorizontal = 0;
			break;
		}
	}
}

void Camera::CheckMove() {
	if (keyPressed & MOVE_FRONT) {
		m_moveVertical = -1.0f;
	}
	if (keyPressed & MOVE_BACK) {
		m_moveVertical = 1.0f;
	}
	if (keyPressed & MOVE_LEFT) {
		m_moveHorizontal = -1.0f;
	}
	if (keyPressed & MOVE_RIGHT) {
		m_moveHorizontal = 1.0f;
	}
	if (keyPressed & ROTATE_UP) {
		m_rotVertical = 1.0f;
	}
	if (keyPressed & ROTATE_DOWN) {
		m_rotVertical = -1.0f;
	}
	if (keyPressed & ROTATE_LEFT) {
		m_rotHorizontal = 1.0f;
	}
	if (keyPressed & ROTATE_RIGHT) {
		m_rotHorizontal = -1.0f;
	}
}

void Camera::CleanUp() {
	if (s_instance) {
		delete s_instance;
		s_instance = NULL;
	}
}