#pragma once
#include "../Utilities/utilities.h"

class Shaders 
{
public:
	GLuint program, vertexShader, fragmentShader;
	char fileVS[260];
	char fileFS[260];
	GLint positionAttribute;
	GLint colorAttribute;
	GLint uvAttribute;
	GLint normalAttribute;
	GLint cubeAttribute;
	GLint uniformLocation;
	GLint mvpUniform;
	GLint rUniform;
	GLint gUniform;
	GLint bUniform;
	GLint heightMapUniform;
	GLint timeUniform;
	GLint fogStartUniform;
	GLint fogLengthUniform;
	GLint fogColorUniform;
	int Init(char * fileVertexShader, char * fileFragmentShader);
	~Shaders();
};