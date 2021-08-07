#pragma once
#include <math.h>
#include "../Utilities/utilities.h"
#include "Camera.h"
#include "Object.h"

class SceneManager {
private:
	static SceneManager* s_instance;
	Object *m_listObject;
	int m_numOfObject;
public:
	SceneManager();
	~SceneManager();
	void Init();
	static SceneManager* GetInstance();
	Object* GetListObject();
	int GetNumOfObject();
	void Draw();
	void CleanUP();
	void MoveCamera(char key, bool bIsPressed);
	void MemoryClear();
	void Update(float deltatime);
};
