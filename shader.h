#pragma once

#define GLEW_STATIC
#include <GLEW/glew.h>

#include <string>
#include <vector>
#include <fstream>

#include "utility.h"

using namespace std;

class Program;

class Shader {
public:
	Shader();
	
	Shader(const string &src, GLenum type);

	void Compile(const string &src, GLenum type);

	operator bool() const;

	~Shader();

	friend class Program;
private:
	GLid res;
};