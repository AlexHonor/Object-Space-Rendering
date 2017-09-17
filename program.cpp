#include "program.h"

Program::Program() {}

Program::Program(const string &vs_src, const string &fs_src) {
	if (!BuildFromSource(vs_src, fs_src)) {
		throw runtime_error("Failed to build Program.");
	}
}

void Program::Use() const {
	glUseProgram(res); GLERR;
}

bool Program::BuildFromFiles(const string &vs_filename, const string &fs_filename) {
    return BuildFromSource(ReadFile(vs_filename),ReadFile(fs_filename));
}

bool Program::BuildFromSource(const string &vs_src, const string &fs_src) {
    Shader vs, fs;
    
    vs.Compile(vs_src, GL_VERTEX_SHADER);
	fs.Compile(fs_src, GL_FRAGMENT_SHADER);

	res = glCreateProgram(); GLERR;
	
	Attach(fs);
	Attach(vs);
	
    BindAttributeLocations();
    glBindFragDataLocation(res, 0, "out_color"); GLERR;

	glLinkProgram(res); GLERR;

	GLint is_linked = 0;
	glGetProgramiv(res, GL_LINK_STATUS, &is_linked); GLERR;
	
	if (!is_linked) {
		GLint max_length = 0;
		glGetProgramiv(res, GL_INFO_LOG_LENGTH, &max_length); GLERR;

		max_length++;
		std::vector<GLchar> info_log(max_length);
		*info_log.rbegin() = '\0';

		glGetProgramInfoLog(res, max_length, &max_length, info_log.data()); GLERR;

		glDeleteProgram(res); GLERR;

		SDL_Log(info_log.data());

		throw runtime_error("Failed to link Program.");
	}

	Detach(fs);
	Detach(vs);

	return true;
}

void Program::BindAttributeLocations() {
    glBindAttribLocation(res, VertexAttribute::POSITION , "a_position" ); GLERR;
    glBindAttribLocation(res, VertexAttribute::COLOR    , "a_color"    ); GLERR;
    glBindAttribLocation(res, VertexAttribute::NORMAL   , "a_normal"   ); GLERR;
    glBindAttribLocation(res, VertexAttribute::TEXCOORD1, "a_texcoord1"); GLERR;
    glBindAttribLocation(res, VertexAttribute::TEXCOORD2, "a_texcoord2"); GLERR;
}

bool Program::TrySetUniform(string name, float44 mat) const {
    GLuint location = glGetUniformLocation(res, name.c_str()); GLERR;
    bool is_valid = IsValid();
    if (location != -1) {
        Use(); GLERR;
        glUniformMatrix4fv(location, 1, GL_FALSE, &mat.a[0][0]); GLERR;
        return true;
    } else {
        return false;
    }
}

bool Program::TrySetUniform(string name, GLuint val) const {
    GLuint location = glGetUniformLocation(res, name.c_str()); GLERR;
    bool is_valid = IsValid();
    if (location != -1) {
        Use(); GLERR;
        glUniform1i(location, val); GLERR;
        return true;
    } else {
        return false;
    }
}

bool Program::IsValid() const {
    bool is_valid = glIsProgram(res); GLERR;
    return is_valid;
}

void Program::Purge() {
    if (!res.IsPurged()) {
        glDeleteProgram(res); GLERR;
        res.Purge();
    }
}

void Program::Attach(const Shader &shader) const {
	glAttachShader(res, shader.res); GLERR;
}
void Program::Detach(const Shader &shader) const {
	glDetachShader(res, shader.res); GLERR;
}

Program::~Program() {
	
}