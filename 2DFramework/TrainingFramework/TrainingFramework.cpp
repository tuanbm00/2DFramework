// TrainingFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Vertex.h"
#include "Shaders.h"
#include "Texture.h"
#include "Globals.h"
#include "Model.h"
#include "Object.h"
#include "Camera.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include <conio.h>
#include <iostream>
#include <box2d/b2_body.h>
#include <box2d/b2_world.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_circle_shape.h>
#include <box2d/b2_edge_shape.h>
#include <box2d/b2_contact.h>
#include <vector>

#define MOVE_FRONT 1
#define MOVE_BACK 1 << 1
#define MOVE_LEFT 1 << 2
#define MOVE_RIGHT 1 << 3

#define ROTATE_LEFT 1 << 4
#define ROTATE_RIGHT 1 << 5
#define DAME 1 << 6
#define DAME_B 1 << 7

//Object obj1, obj2, skyBox;
Object obj, land, bullet, wall, boss;
b2Vec2 gravity(0.0f, 0.0f);
b2World world(gravity);
b2Body* body;
b2Body* bossBody;
b2Body* listBulletBody[100];
int keyPressed = 0, num = 0, m_add = 0;
float id = 10.0f;
float posX = 100.0f, posY = 100.0f;
float m_moveVertical = 0, m_moveHorizontal = 0;
Object listBullet[100];
short CATEGORY_PLAYER = 0x0001; // 0000000000000001 
short CATEGORY_WALL = 0x0002;
short CATEGORY_BULLET = 0x0004;
short CATEGORY_BOSS = 0x0008;

int Init(ESContext *esContext)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	b2BodyDef groundBodyDef;
	groundBodyDef.type = b2_staticBody;
	groundBodyDef.position.Set(-10000.0f, 700.0f);
	b2Body* groundBody = world.CreateBody(&groundBodyDef);
	b2PolygonShape groundBox;
	groundBox.SetAsBox(20000.0f, 1.0f);
	b2FixtureDef groundTexture;
	groundTexture.shape = &groundBox;
	groundTexture.filter.categoryBits = CATEGORY_WALL;
	groundTexture.filter.maskBits = CATEGORY_PLAYER | CATEGORY_BULLET | CATEGORY_BOSS;
	groundBody->CreateFixture(&groundTexture);

	b2BodyDef wallBodyDef;
	wallBodyDef.type = b2_staticBody;
	wallBodyDef.position.Set(680.0f, 700.0f);
	b2Body* wallBody = world.CreateBody(&wallBodyDef);
	b2PolygonShape wallBox;
	wallBox.SetAsBox(240.0f, 400.0f);
	b2FixtureDef wallFixture;
	wallFixture.shape = &wallBox;
	wallFixture.filter.categoryBits = CATEGORY_WALL;
	wallFixture.filter.maskBits = CATEGORY_PLAYER | CATEGORY_BULLET | CATEGORY_BOSS;
	wallBody->CreateFixture(&wallFixture);

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 400.0f);
	body = world.CreateBody(&bodyDef);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(240.0f, 240.0f);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;
	fixtureDef.filter.categoryBits = CATEGORY_PLAYER;
	fixtureDef.filter.maskBits = CATEGORY_WALL | CATEGORY_BULLET | CATEGORY_BOSS;
	body->CreateFixture(&fixtureDef);
	body->SetLinearVelocity(b2Vec2(00.0f, -100.0f));

	b2BodyDef bossBodyDef;
	bossBodyDef.type = b2_dynamicBody;
	bossBodyDef.position.Set(3000.0f, 400.0f);
	bossBody = world.CreateBody(&bossBodyDef);
	b2PolygonShape bossBox;
	bossBox.SetAsBox(480.0f, 480.0f);
	b2FixtureDef bossFixture;
	bossFixture.shape = &bossBox;
	bossFixture.filter.categoryBits = CATEGORY_BOSS;
	bossFixture.filter.maskBits = CATEGORY_PLAYER | CATEGORY_BULLET | CATEGORY_WALL;
	bossBody->CreateFixture(&bossFixture);

	Camera::GetInstance()->Init(1.0f, -40.0f, 30.0f);
	Camera::GetInstance()->SetPosition(posX, posY, 15.0f);
	Camera::GetInstance()->SetTarget(posX, posY, 0);
	Camera::GetInstance()->SetUp(0, 1, 0);
	Camera::GetInstance()->SetSpeed(5, 2);
	Vector3 pos = Vector3(posX, posY, 10.0f);
	Vector3 tar = Vector3(posX, posY, 0.0f);
	Vector3 up = Vector3(0, 1, 0);

	obj.SetnumOfTexture(1);
	obj.SetPosition(0, 0, 0);
	obj.SetRotation(0, 0, 0);
	obj.SetScale(1, 1, 1);

	land.SetnumOfTexture(1);
	land.SetPosition(0, 760, 0);
	land.SetRotation(0, 0, 0);
	land.SetScale(1, 1, 1);

	wall.SetnumOfTexture(1);
	wall.SetPosition(700, 550, 0);
	wall.SetRotation(0, 0, 0);
	wall.SetScale(1, 1, 1);

	boss.SetnumOfTexture(1);
	boss.SetPosition(3000, 400, 0);
	boss.SetRotation(0, 0, 0);
	boss.SetScale(1, 1, 1);
	boss.SetID(10);
	obj.Init2D("../Resources/homaru.tga", 1, 1);
	land.Init2D("../Resources/Dirt.tga", 0.05f, 4.0f);
	wall.Init2D("../Resources/Dirt.tga", 1.0f, 1.0f);
	boss.Init2D("../Resources/boss.tga", 0.5f, 0.5f);

	for (auto b = world.GetBodyList(); b; b = b->GetNext())
	{
		b2Vec2 pos = b->GetPosition();
	//	printf("%f %f \n", pos.x, pos.y);
	}

	//b2BodyDef bd;
	//bd.type = b2_dynamicBody;
	
	return 0;
}

void Draw ( ESContext *esContext )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	obj.Draw2D();
	land.Draw2D();
	wall.Draw2D();
	if(boss.m_iObjectID > 0)
		boss.Draw2D();
	for (int i = 0; i < num; i++) {
		listBullet[i].Draw2D();
	}
	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
}

void Update(ESContext *esContext, float deltaTime)
{
	id += deltaTime;
	if (keyPressed & MOVE_FRONT) {
		m_moveVertical = -10.0f;
	}
	if (keyPressed & MOVE_BACK) {
		m_moveVertical = 0.0f;
	}
	if (keyPressed & MOVE_LEFT) {
		m_moveHorizontal = -10.0f;
	}
	if (keyPressed & MOVE_RIGHT) {
		m_moveHorizontal = 10.0f;
	}
	if (keyPressed & DAME) {
		m_add = 1;
	}
	if (keyPressed & DAME_B) {
		m_add = 2;
	}
	if (m_moveVertical == 0) {
		m_moveVertical = 10.0f;
	}
	body->SetLinearVelocity(b2Vec2(m_moveHorizontal, m_moveVertical));
	if (m_add > 0 && id >= 0.5f) {
		b2Vec2 pos = body->GetPosition();
		listBullet[num].SetnumOfTexture(1);
		listBullet[num].SetPosition(pos.x + 300, pos.y + 50, 0);
		listBullet[num].SetRotation(0, 0, 0);
		listBullet[num].SetScale(1, 1, 1);
		listBullet[num].SetID(m_add);
		if(m_add == 1)
			listBullet[num].Init2D("../Resources/bluebullet.tga", 4, 4);
		else 
			listBullet[num].Init2D("../Resources/bazokabullet.tga", 4, 4);

		b2BodyDef bullet;
		bullet.type = b2_dynamicBody;
		bullet.position.Set(pos.x + 300, pos.y + 50);
		listBulletBody[num] = world.CreateBody(&bullet);
		b2PolygonShape bulletBox;
		bulletBox.SetAsBox(60.0f, 60.0f);
		b2FixtureDef fixtureBullet;
		fixtureBullet.shape = &bulletBox;
		fixtureBullet.density = 1.0f;
		fixtureBullet.friction = 0.0f;
		fixtureBullet.filter.categoryBits = CATEGORY_BULLET;
		fixtureBullet.filter.maskBits = CATEGORY_WALL | CATEGORY_BOSS;
		listBulletBody[num]->CreateFixture(&fixtureBullet);
		float h = -20 * (m_add - 1);
		listBulletBody[num]->SetLinearVelocity(b2Vec2(10.0f, h));
		num++;
		id = 0;
	//	printf("%d\n", num);
	}
	int n = int(deltaTime / 0.001);
//	printf("%d\n", n);
	int32 velocityIterations = 6;
	int32 positionIterations = 2;
	for (int x = 0; x < n; x++) {
		world.Step(0.4f, velocityIterations, positionIterations);
		for (int i = 0; i < num; i++) {
			if (listBullet[i].m_iObjectID == 2) {
				b2Vec2 v = listBulletBody[i]->GetLinearVelocity();
				listBulletBody[i]->SetLinearVelocity(b2Vec2(v.x, v.y + 0.005f));
			}
			int iTrue = 0;
			for (b2ContactEdge* edge = listBulletBody[i]->GetContactList(); edge; edge = edge->next) {
				b2Fixture* a = edge->contact->GetFixtureA();
				b2Fixture* b = edge->contact->GetFixtureB();
				if (a->GetFilterData().categoryBits == CATEGORY_WALL || b->GetFilterData().categoryBits == CATEGORY_WALL) {
					for (int j = i; j < num; j++) {
						if (j == i) {
							delete[] listBullet[i].m_texture;
							delete[] listBullet[i].textureID;
							world.DestroyBody(listBulletBody[i]);
						}
						listBullet[j] = listBullet[j + 1];
						listBulletBody[j] = listBulletBody[j + 1];
					}

					num--;
					iTrue = 1;
					break;
				}
				if (a->GetFilterData().categoryBits == CATEGORY_BOSS || b->GetFilterData().categoryBits == CATEGORY_BOSS) {
					b2Vec2 pos;
					boss.SetID(boss.m_iObjectID - 1);
					if (boss.m_iObjectID == 0) {
						world.DestroyBody(bossBody);
						break;
					}
					if (a->GetFilterData().categoryBits == CATEGORY_BOSS) {
						pos = a->GetBody()->GetPosition();
					}
					else {
						pos = b->GetBody()->GetPosition();
					}
					pos = b2Vec2(pos.x + 630.f, listBullet[i].m_position.y);
					iTrue = listBullet[i].m_iObjectID;
					for (int j = i; j < num; j++) {
						if (j == i) {
							delete[] listBullet[i].m_texture;
							delete[] listBullet[i].textureID;
							world.DestroyBody(listBulletBody[i]);
						}
						listBullet[j] = listBullet[j + 1];
						listBulletBody[j] = listBulletBody[j + 1];
					}
					num--;
					if (iTrue == 1) {
						listBullet[num].SetnumOfTexture(1);
						listBullet[num].SetPosition(pos.x, pos.y - 60, 0);
						listBullet[num].SetRotation(0, 0, 0);
						listBullet[num].SetScale(1, 1, 1);
						listBullet[num].SetID(1);
						listBullet[num].Init2D("../Resources/bluebullet.tga", 4, 4);
						b2BodyDef bullet1;
						bullet1.type = b2_dynamicBody;
						bullet1.position.Set(pos.x, pos.y - 60);
						listBulletBody[num] = world.CreateBody(&bullet1);
						b2PolygonShape bulletBox1;
						bulletBox1.SetAsBox(60.0f, 60.0f);
						b2FixtureDef fixtureBullet1;
						fixtureBullet1.shape = &bulletBox1;
						fixtureBullet1.filter.categoryBits = CATEGORY_BULLET;
						fixtureBullet1.filter.maskBits = CATEGORY_WALL | CATEGORY_BOSS;
						listBulletBody[num]->CreateFixture(&fixtureBullet1);
						listBulletBody[num]->SetLinearVelocity(b2Vec2(20.0f, -4.0f));
						num++;

						listBullet[num].SetnumOfTexture(1);
						listBullet[num].SetPosition(pos.x, pos.y, 0);
						listBullet[num].SetRotation(0, 0, 0);
						listBullet[num].SetScale(1, 1, 1);
						listBullet[num].SetID(1);
						listBullet[num].Init2D("../Resources/bluebullet.tga", 4, 4);
						b2BodyDef bullet2;
						bullet2.type = b2_dynamicBody;
						bullet2.position.Set(pos.x, pos.y);
						listBulletBody[num] = world.CreateBody(&bullet2);
						b2PolygonShape bulletBox2;
						bulletBox2.SetAsBox(60.0f, 60.0f);
						b2FixtureDef fixtureBullet2;
						fixtureBullet2.shape = &bulletBox2;
						fixtureBullet2.filter.categoryBits = CATEGORY_BULLET;
						fixtureBullet2.filter.maskBits = CATEGORY_WALL | CATEGORY_BOSS;
						listBulletBody[num]->CreateFixture(&fixtureBullet2);
						listBulletBody[num]->SetLinearVelocity(b2Vec2(20.0f, 0.0f));
						num++;

						listBullet[num].SetnumOfTexture(1);
						listBullet[num].SetPosition(pos.x, pos.y+60, 0);
						listBullet[num].SetRotation(0, 0, 0);
						listBullet[num].SetScale(1, 1, 1);
						listBullet[num].SetID(1);
						listBullet[num].Init2D("../Resources/bluebullet.tga", 4, 4);
						b2BodyDef bullet3;
						bullet3.type = b2_dynamicBody;
						bullet3.position.Set(pos.x, pos.y + 60);
						listBulletBody[num] = world.CreateBody(&bullet3);
						b2PolygonShape bulletBox3;
						bulletBox3.SetAsBox(60.0f, 60.0f);
						b2FixtureDef fixtureBullet3;
						fixtureBullet3.shape = &bulletBox3;
						fixtureBullet3.filter.categoryBits = CATEGORY_BULLET;
						fixtureBullet3.filter.maskBits = CATEGORY_WALL | CATEGORY_BOSS;
						listBulletBody[num]->CreateFixture(&fixtureBullet3);
						listBulletBody[num]->SetLinearVelocity(b2Vec2(20.0f, 4.0f));
						num++;
					}
					break;
				}
			}
			if (iTrue > 0) {
				continue;
			}
			listBullet[i].Update(2*deltaTime);
			b2Vec2 pos = listBulletBody[i]->GetPosition();
			listBullet[i].SetPosition(pos.x, pos.y, 0);
			if (listBullet[i].xlong > 50 && listBullet[i].m_iObjectID == 1) {
				for (int j = i; j < num; j++) {
					if (j == i) {
						delete[] listBullet[i].m_texture;
						delete[] listBullet[i].textureID;
						world.DestroyBody(listBulletBody[i]);
					}
					listBullet[j] = listBullet[j + 1];
					listBulletBody[j] = listBulletBody[j + 1];
				}
				num--;
			}
		}
		for (int i = 0; i < num; i++) {
			listBullet[i].Draw2D();
		}
		b2Vec2 position = body->GetPosition();
		obj.SetPosition(position.x, position.y, 0);
		Camera::GetInstance()->SetPosition(position.x, posY, 20.0f);
		Camera::GetInstance()->SetTarget(position.x, posY, 0);
		obj.Draw2D();
	}

	b2Vec2 position = body->GetPosition();
}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{
//	Camera::GetInstance()->Move(key, bIsPressed);
	if (bIsPressed) {
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
		case 'J':
		case 'j':
			keyPressed = keyPressed | DAME;
			break;
		case 'K':
		case 'k':
			keyPressed = keyPressed | DAME_B;
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
		case 'j':
		case 'J':
			keyPressed = keyPressed ^ DAME;
			m_add = 0;
			break;
		case 'K':
		case 'k':
			keyPressed = keyPressed ^ DAME_B;
			m_add = 0;
			break;
		}
	}
}

void CleanUp()
{
	obj.CleanUp();
	land.CleanUp();
	wall.CleanUp();
	boss.CleanUp();
}

int _tmain(int argc, _TCHAR* argv[])
{
	ESContext esContext;

    esInitContext ( &esContext );

	esCreateWindow ( &esContext, "Hello Triangle", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if ( Init ( &esContext ) != 0 )
		return 0;

	esRegisterDrawFunc ( &esContext, Draw );
	esRegisterUpdateFunc ( &esContext, Update );
	esRegisterKeyFunc ( &esContext, Key);

	esMainLoop ( &esContext );

	//releasing OpenGL resources
	CleanUp();
	SceneManager::GetInstance()->CleanUP();
	Camera::GetInstance()->CleanUp();
	delete[] obj.textureID;
	delete[] obj.m_texture;
	delete[] land.textureID;
	delete[] land.m_texture;
	delete[] wall.textureID;
	delete[] wall.m_texture;
	delete[] boss.textureID;
	delete[] boss.m_texture;

	//identifying memory leaks
	MemoryDump();
	printf("Press any key...\n");
	_getch();

	return 0;
}

