#pragma once

#include "../Utilities/utilities.h"

class Texture {
//private:
//	static Texture* s_Instance;
public:
	Texture();
	~Texture();
	void Init(char* filename);
	void Init(char* filename, int i);
	void loadCube(char* rightfile, char* leftfile, char* topfile, char* botfile, char* frontfile, char* backfile);
	void loadCube(char* filename);
//	static Texture* GetInstance();
};