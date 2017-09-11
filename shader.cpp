#include "shader.h"

Shader::Shader() {}

Shader::Shader(const string &src, GLenum type) {
	Compile(src, type);
}

void Shader::Compile(const string &src, GLenum type) {
	res = glCreateShader(type); GLERR;

	const GLchar *source = src.c_str();
	glShaderSource(res, 1, &source, nullptr); GLERR;

	glCompileShader(res); GLERR;

	GLint is_compiled = 0;
	GLuint prog = res;
	glGetShaderiv(res, GL_COMPILE_STATUS, &is_compiled); GLERR;
	
	if (!is_compiled) {
		GLint max_length = 0;
		glGetShaderiv(res, GL_INFO_LOG_LENGTH, &max_length); GLERR;

		vector<GLchar> log(max_length + 1);
		glGetShaderInfoLog(res, max_length, &max_length, log.data()); GLERR;

		SDL_Log(log.data());

		throw runtime_error("Failed to compile Program.");
	}
}
	
Shader::operator bool() const {
	return res.IsPurged();
}

Shader::~Shader() {
	if (!res.IsPurged()) {
		glDeleteShader(res); GLERR;
		res.Purge();
	}
}