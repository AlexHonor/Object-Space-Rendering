#pragma once

#include "shader.h"

class Program {
public:
    enum VertexAttribute {
        POSITION,
        COLOR,
        NORMAL,
        TEXCOORD1,
        TEXCOORD2,
        NONE
    };
    
    Program();
	Program(const string &vs_filename, const string &fs_filename);

	void Use();

    void BindAttributeLocations();
	bool BuildFromSource(const string &vs_src, const string &fs_src);
    bool BuildFromFiles (const string &vs_src, const string &fs_src);
    bool TrySetUniform(string name, float44 mat);
    bool TrySetUniform(string name, GLuint mat);
    void Purge();

    bool IsValid() const;

	~Program();

protected:
	void Attach(const Shader &prog) const;
	void Detach(const Shader &prog) const;

	string name;

	GLResource res;
};